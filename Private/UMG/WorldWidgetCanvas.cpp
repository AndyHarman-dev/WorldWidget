// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/WorldWidgetCanvas.h"
#include "ValidateMacro.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Slate/SGameLayerManager.h"

#define LOG_CATEGORY_NAME LogLayerScreenWidget
#include "EasyLog.h"
#include "Interfaces/WorldWidgetProvider.h"
#include "Miscellaneous/WorldToScreenLocationProjector.h"

uint32 FAutoDestroyWidgetPtrBase::Release() const
{
	if (GetRefCount() == 1)
	{
		if (Destructor)
		{
			Destructor(Widget.Get());
		}
	}
	
	return Super::Release();
}

void FAutoDestroyWidgetPtrBase::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(Widget);
}

void UWorldWidgetCanvas::RedrawWith(const TArray<TScriptInterface<IWorldWidgetProvider>>& Providers)
{
	if (Providers.IsEmpty())
	{
		LOG_ERROR("Failed to intialized widget. Components are empty!");
		return;
	}

	Entries.Empty();
	Entries.Reserve(Providers.Num());

	for (const auto& Component : Providers)
	{
		AddUnique(Component);
	}
}

void UWorldWidgetCanvas::AddUnique(const TScriptInterface<IWorldWidgetProvider>& Provider)
{
	if (!ValidateMsfg(IsValid(Provider.GetObject()), LOG_CATEGORY_NAME, "Invalid component!"))
	{
		return;
	}

	if (Contains(Provider))
	{
		LOG_WARNING("Provider: {0} already exists! Skipping...", Provider.GetObject());
		return;
	}
	
	Entries.Add(MakeNewEntry(Provider));
}

void UWorldWidgetCanvas::Remove(const TScriptInterface<IWorldWidgetProvider>& Provider)
{
	if (!ValidateMsfg(IsValid(Provider.GetObject()), LOG_CATEGORY_NAME, "Invalid component!"))
	{
		return;
	}

	if (!ValidateMsfg(Contains(Provider), LOG_CATEGORY_NAME, "Provider not found!"))
	{
		return;
	}

	Entries.RemoveAll([Provider](const FEntry& OneOfEntries)
	{
		return Provider == OneOfEntries.Provider;
	});
}

void UWorldWidgetCanvas::Clear()
{
	Entries.Empty();
}

bool UWorldWidgetCanvas::Contains(const TScriptInterface<IWorldWidgetProvider>& Provider)
{
	if (!ValidateMsfg(IsValid(Provider.GetObject()), LOG_CATEGORY_NAME, "Invalid component!"))
	{
		return false;
	}

	return Entries.ContainsByPredicate([Provider](const FEntry& OneOfEntries)
	{
		return Provider == OneOfEntries.Provider;
	});
}

void UWorldWidgetCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Projector = NewObject<UWorldToScreenLocationProjector>(this);
	Projector->Initialize(GetOwningPlayer());
}

FEntry UWorldWidgetCanvas::MakeNewEntry(const TScriptInterface<IWorldWidgetProvider>& Provider)
{
	FEntry NewEntry = FEntry();
	NewEntry.Provider = Provider;

	const auto Widget = IWorldWidgetProvider::Execute_GetWidget(Provider.GetObject());
	NewEntry.Widget = ToRefCountPtr(Widget);
	NewEntry.Pivot = IWorldWidgetProvider::Execute_GetPivot(Provider.GetObject());
	
	NewEntry.ContainerWidget = MakeContainerWidget();
	NewEntry.ContainerWidget->AddChild(Widget);

	NewEntry.Slot = Canvas->AddChildToCanvas(NewEntry.ContainerWidget);
	ApplyCanvasParametersTo(NewEntry.Slot);
	return NewEntry;
}

UBorder* UWorldWidgetCanvas::MakeContainerWidget()
{
	auto NewContainer = NewObject<UBorder>();

	FSlateBrush Brush = FSlateBrush();
	Brush.DrawAs = ESlateBrushDrawType::NoDrawType;

	NewContainer->SetBrush(Brush);
	return NewContainer;
}

FAutoDestroyWidgetPtr UWorldWidgetCanvas::ToRefCountPtr(UWidget* Widget)
{
	auto CustomWidgetDestructor = [This = TWeakObjectPtr(this)](UWidget* Widget)
	{
		if (This.IsValid() && !This.IsStale())
		{
			This->Canvas->RemoveChild(Widget);
		}
	};
	
	return MakeRefCount<FAutoDestroyWidgetPtrBase>(Widget, CustomWidgetDestructor);
}

void UWorldWidgetCanvas::ApplyCanvasParametersTo(UCanvasPanelSlot* Slot)
{
	Slot->SetAutoSize(Parameters.SlotSize.IsZero() || bUpdateSlotSettingsOnTick);
	Slot->SetAlignment(Parameters.SlotAlignment);
	Slot->SetAnchors(Parameters.SlotAnchors);
	Slot->SetSize(Parameters.SlotSize);
}

void UWorldWidgetCanvas::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	this->MyGeometry = &MyGeometry;

	PositionEntries();
	OrderWidgetsByProximityToCamera();
	
	if (GSlateIsOnFastUpdatePath)
	{
		ProcessSlateIsOnFastUpdatePath();
	}
}

void UWorldWidgetCanvas::PositionEntries()
{
	Projector->UpdateProjectionData();
	
	for ( auto It = Entries.CreateIterator(); It; ++It )
	{
		auto& Entry = *It;
		
		if (!IWorldWidgetProvider::Execute_IsVisible(Entry.Provider.GetObject()))
		{
			Canvas->RemoveChild(Entry.ContainerWidget);
			It.RemoveCurrent();
			continue;
		}
		
		const auto WorldLocation = IWorldWidgetProvider::Execute_GetWorldLocation(Entry.Provider.GetObject());
		if (bZOrderWidgetsByProximityToCamera)
		{
			Entry.Distance = CalculateDistanceBetweenPlayerAnd(WorldLocation);
		}

		Projector->SetWorldLocation(WorldLocation);
		if (!Projector->Project())
		{
			Entry.ContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		if ((*Entry.Widget)->GetPixelSnapping() == EWidgetPixelSnapping::SnapToPixel)
		{
			Projector->RoundScreenPosition();
		}
		
		Entry.ContainerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetCanvasSlotPositionAndParameters(Entry);
	}
}

float UWorldWidgetCanvas::CalculateDistanceBetweenPlayerAnd(FVector WorldLocation)
{
	const auto LPawn =  GetOwningPlayer()->GetPawn();
	if (!IsValid(LPawn))
	{
		return 0.f;
	}
	
	return FVector::DistSquared(WorldLocation, LPawn->GetActorLocation());
}

void UWorldWidgetCanvas::SetCanvasSlotPositionAndParameters(const FEntry& Entry)
{
	if (!IsValid(Entry.Slot))
	{
		return;
	}
	
	FVector2D LocalPosition = Projector->GetLocalPositionFromViewportPosition(*MyGeometry);

	if ( Entry.Provider )
	{
		Entry.Slot->SetAutoSize(Parameters.SlotSize.IsZero() || bUpdateSlotSettingsOnTick);
		Entry.Slot->SetOffsets(FMargin(LocalPosition.X, LocalPosition.Y, Parameters.SlotSize.X, Parameters.SlotSize.Y));
		Entry.Slot->SetAnchors(FAnchors(0, 0, 0, 0));
		Entry.Slot->SetAlignment(Entry.Pivot);
	}
	else
	{
		Entry.Slot->SetAutoSize(Parameters.SlotSize.IsZero());
		Entry.Slot->SetOffsets(FMargin(LocalPosition.X, LocalPosition.Y, Parameters.SlotSize.X, Parameters.SlotSize.Y));
		Entry.Slot->SetAnchors(FAnchors(0, 0, 0, 0));
		Entry.Slot->SetAlignment(Parameters.SlotAlignment);
	}
}

void UWorldWidgetCanvas::OrderWidgetsByProximityToCamera()
{
	if (bZOrderWidgetsByProximityToCamera)
	{
		Entries.Sort([](const auto& InA, const auto& InB)
		{
			return InA.Distance > InB.Distance;
		});

		int32 _count = 0;
		for (auto& LEntry : Entries)
		{
			if (IsValid(LEntry.Slot))
			{
				LEntry.Slot->SetZOrder(_count);
			}

			_count++;
		}
	}
}

void UWorldWidgetCanvas::ProcessSlateIsOnFastUpdatePath()
{
	// Hide everything if we are unable to do any of the work.
	for (auto& Entry : Entries)
	{
		Entry.ContainerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
