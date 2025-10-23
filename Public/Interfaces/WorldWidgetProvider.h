// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WorldWidgetProvider.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWorldWidgetProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WORLDWIDGET_API IWorldWidgetProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Should return a widget that is expected to be drawn at projected location on 2D screen
	UFUNCTION(BlueprintNativeEvent, Category="WorldWidgetProvider")
	UUserWidget* GetWidget();
	UFUNCTION(BlueprintNativeEvent, Category="WorldWidgetProvider")
	FVector GetWorldLocation();
	UFUNCTION(BlueprintNativeEvent, Category="WorldWidgetProvider")
	FVector2D GetPivot();
	UFUNCTION(BlueprintNativeEvent, Category="WorldWidgetProvider")
	bool IsVisible();

	UFUNCTION(BlueprintNativeEvent, Category="WorldWidgetProvider")
	void NotifyWidgetConstructed();
};
