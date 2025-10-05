// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultType/Result.h"
#include "WorldWidgetCanvas.generated.h"

class UWorldToScreenLocationProjector;
class IWorldWidgetProvider;
class USpacer;
class UCanvasPanel;
class UCanvasPanelSlot;
class UBorder;

class FAutoDestroyWidgetPtrBase : public TRefCountingMixin<FAutoDestroyWidgetPtrBase>, public FGCObject
{
	using Super = TRefCountingMixin;
public:
	FAutoDestroyWidgetPtrBase(const FAutoDestroyWidgetPtrBase&) = delete;
	FAutoDestroyWidgetPtrBase& operator=(const FAutoDestroyWidgetPtrBase&) = delete;
	FAutoDestroyWidgetPtrBase(TWeakObjectPtr<UWidget> Widget, TFunction<void(UWidget*)> DestructorFunc)
	:
	Widget(Widget),
	Destructor(MoveTemp(DestructorFunc))
	{}

	uint32 Release() const;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override
	{
		return TEXT("FAutoDestroyWidgetPtrBase");
	}

	UWidget* operator->() const 
	{
		return Widget.Get();
	}

private:
	TWeakObjectPtr<UWidget> Widget;
	TFunction<void(UWidget*)> Destructor;
};
using FAutoDestroyWidgetPtr = TRefCountPtr<FAutoDestroyWidgetPtrBase>;


USTRUCT(BlueprintType)
struct FCanvasSlotParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="WorldWidget")
	FAnchors SlotAnchors;

	UPROPERTY(EditDefaultsOnly, Category="WorldWidget")
	FVector2D SlotAlignment;
	
	UPROPERTY(EditDefaultsOnly, Category="WorldWidget")
	FVector2D SlotSize;
};

USTRUCT()
struct FEntry
{
	GENERATED_BODY()
	bool bRemoving = false;

	UPROPERTY(Transient)
	TScriptInterface<IWorldWidgetProvider> Provider;

	FVector2D Pivot;
	
	UPROPERTY(Transient)
	UBorder* ContainerWidget;
	
	FAutoDestroyWidgetPtr Widget;

	UPROPERTY(Transient)
	UCanvasPanelSlot* Slot;

	float Distance;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDWIDGET_API UWorldWidgetCanvas : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="WorldWidget")
	void RedrawWith(const TArray<TScriptInterface<IWorldWidgetProvider>>& Providers);
	UFUNCTION(BlueprintCallable, Category="WorldWidget")
	void AddUnique(const TScriptInterface<IWorldWidgetProvider>& Provider);
	UFUNCTION(BlueprintCallable, Category="WorldWidget")
	void Remove(const TScriptInterface<IWorldWidgetProvider>& Provider);
	UFUNCTION(BlueprintCallable, Category="WorldWidget")
	void Clear();
	UFUNCTION(BlueprintPure, Category="WorldWidget")
	bool Contains(const TScriptInterface<IWorldWidgetProvider>& Provider);

	virtual void NativeOnInitialized() override;
private:
	UPROPERTY()
	TArray<FEntry> Entries;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="WorldWidget")
	TSubclassOf<UWidget> EntryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="WorldWidget|Slot")
	FCanvasSlotParameters Parameters;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category="WorldWidget|Slot")
	bool bZOrderWidgetsByProximityToCamera = false;
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category="WorldWidget|Slot")
	bool bUpdateSlotSettingsOnTick = true;

	const FGeometry* MyGeometry = nullptr;

	UPROPERTY(Transient)
	UWorldToScreenLocationProjector* Projector;
	
	FEntry MakeNewEntry(const TScriptInterface<IWorldWidgetProvider>& Provider);
	UBorder* MakeContainerWidget();
	FAutoDestroyWidgetPtr ToRefCountPtr(UWidget* Widget);
	void ApplyCanvasParametersTo(UCanvasPanelSlot* Slot);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void PositionEntries();
	float CalculateDistanceBetweenPlayerAnd(FVector WorldLocation);
	void SetCanvasSlotPositionAndParameters(const FEntry& Entry);
	void OrderWidgetsByProximityToCamera();
	void ProcessSlateIsOnFastUpdatePath();
};


