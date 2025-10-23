// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WorldWidgetComponent.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UWorldWidgetComponent::GetWidget_Implementation()
{
	return Widget;
}

void UWorldWidgetComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();
	Event_OnVisibilityChanged.Broadcast(Super::IsVisible());
}

void UWorldWidgetComponent::PostInitProperties()
{
	Super::PostInitProperties();
	if (WidgetClass)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}
}