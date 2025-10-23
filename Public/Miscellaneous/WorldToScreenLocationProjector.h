// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldToScreenLocationProjector.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDWIDGET_API UWorldToScreenLocationProjector : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	void Initialize(APlayerController* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	void SetWorldLocation(FVector WorldLocation);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Project"), Category = "WorldToScreenLocationProjector")
	bool BP_Project_NoArgs()
	{
		return Project();
	}
	bool Project();
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Project (With World Location)"), Category = "WorldToScreenLocationProjector")
	bool BP_Project_WithWorldLocation(FVector WorldLocation)
	{
		return Project(WorldLocation);
	}
	bool Project(FVector WorldLocation);

	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	void RoundScreenPosition();

	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	FVector GetViewportPosition();
	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	FVector2D GetLocalPositionFromViewportPosition(const FGeometry& Geometry);

	UFUNCTION(BlueprintCallable, Category = "WorldToScreenLocationProjector")
	void UpdateProjectionData();
private:
	struct FProjectionData
	{
		FSceneViewProjectionData ProjectionData;
		FMatrix ViewProjectionMatrix;
		bool bHasProjectionData = false;
	};
	
	FProjectionData ProjectionData;
	FVector WorldLocation;
	UPROPERTY()
	TObjectPtr<UGameViewportClient> ViewportClient;
	UPROPERTY()
	TObjectPtr<ULocalPlayer> Player;
	FGeometry ViewportGeometry;
	FVector2D ProjectedScreenPosition;
};
