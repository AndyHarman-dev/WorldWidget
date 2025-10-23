// Fill out your copyright notice in the Description page of Project Settings.


#include "Miscellaneous/WorldToScreenLocationProjector.h"
#define LOG_CATEGORY_NAME LogWorldToScreenLocationProjector
#include "EasyLog.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Slate/SGameLayerManager.h"

void UWorldToScreenLocationProjector::Initialize(APlayerController* OwningPlayer)
{
	auto LocalPlayer = OwningPlayer->GetLocalPlayer();
	this->ViewportClient = LocalPlayer->ViewportClient;
	this->Player = LocalPlayer;
}

void UWorldToScreenLocationProjector::SetWorldLocation(FVector WorldLocation)
{
	this->WorldLocation = WorldLocation;
}

bool UWorldToScreenLocationProjector::Project()
{
	return Project(WorldLocation);
}

bool UWorldToScreenLocationProjector::Project(FVector WorldLocation)
{
	if (!ProjectionData.bHasProjectionData)
	{
		UpdateProjectionData();
		if (!ProjectionData.bHasProjectionData)
		{
			return false;
		}
	}

	return FSceneView::ProjectWorldToScreen(WorldLocation, ProjectionData.ProjectionData.GetConstrainedViewRect(), ProjectionData.ViewProjectionMatrix, ProjectedScreenPosition);
}

void UWorldToScreenLocationProjector::RoundScreenPosition()
{
	ProjectedScreenPosition = FVector2D(FMath::RoundToDouble(ProjectedScreenPosition.X), FMath::RoundToDouble(ProjectedScreenPosition.Y));
}

FVector UWorldToScreenLocationProjector::GetViewportPosition()
{
	const double ViewportDist = FVector::Dist(ProjectionData.ProjectionData.ViewOrigin, WorldLocation);
	
	FVector2D ViewportPosition2D;
	USlateBlueprintLibrary::ScreenToViewport(Player, ProjectedScreenPosition, OUT ViewportPosition2D);

	return FVector(ViewportPosition2D.X, ViewportPosition2D.Y, ViewportDist);
}

FVector2D UWorldToScreenLocationProjector::GetLocalPositionFromViewportPosition(const FGeometry& Geometry)
{
	const auto ViewportPosition = GetViewportPosition();
	const auto ViewportGeometry = ViewportClient->GetGameLayerManager()->GetViewportWidgetHostGeometry();
	
	FVector2D AbsoluteProjectedLocation = ViewportGeometry.LocalToAbsolute(FVector2D(ViewportPosition.X, ViewportPosition.Y));
	return Geometry.AbsoluteToLocal(AbsoluteProjectedLocation);
}

void UWorldToScreenLocationProjector::UpdateProjectionData()
{
	if (!IsValid(ViewportClient))
	{
		LOG_ERROR("Invalid viewport client!");
	}

	if (!IsValid(Player))
	{
		LOG_ERROR("Invalid player!");
	}

	ProjectionData.bHasProjectionData = Player->GetProjectionData(ViewportClient->Viewport, /*out*/ ProjectionData.ProjectionData);
	if (ProjectionData.bHasProjectionData)
	{
		ProjectionData.ViewProjectionMatrix = ProjectionData.ProjectionData.ComputeViewProjectionMatrix();
	}
}
