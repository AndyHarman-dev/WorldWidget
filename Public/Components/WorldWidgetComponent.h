// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interfaces/WorldWidgetProvider.h"
#include "WorldWidgetComponent.generated.h"


class UBaseUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisibilitySignature, bool, Visible);

UCLASS(ClassGroup=(WorldWidget), meta=(BlueprintSpawnableComponent), Blueprintable)
class WORLDWIDGET_API UWorldWidgetComponent : public USceneComponent, public IWorldWidgetProvider
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "WorldWidget")
	FOnVisibilitySignature Event_OnVisibilityChanged;

	virtual UUserWidget* GetWidget_Implementation() override;
	virtual bool IsVisible_Implementation() override
	{
		return Super::IsVisible();
	}
	virtual FVector2D GetPivot_Implementation() override
	{
		return Pivot;
	}
	virtual FVector GetWorldLocation_Implementation() override
	{
		return GetComponentLocation();
	}

protected:	
	virtual void OnVisibilityChanged() override;

	UPROPERTY(EditAnywhere, Category = "WorldWidget")
	FVector2D Pivot = FVector2D{ 0.5f, 0.5f };
	
	UPROPERTY(EditAnywhere, Category = "WorldWidget")
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(BlueprintReadOnly, Category = "WorldWidget")
	TObjectPtr<UUserWidget> Widget;
	virtual void PostInitProperties() override;
};
