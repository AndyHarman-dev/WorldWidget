// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WorldWidgetComponent.h"

#include "BaseUserWidget/BaseUserWidget.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UWorldWidgetComponent::GetWidget_Implementation()
{
	return Widget;
}

void UWorldWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	Widget->OnConstruct.AddDynamic(this, &ThisClass::OnWidgetConstructed);
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
		Widget = CreateWidget<UBaseUserWidget>(GetWorld(), WidgetClass);
	}
}