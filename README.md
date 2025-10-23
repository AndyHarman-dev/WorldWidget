# WorldWidget Documentation

## Project Title & Description

**WorldWidget**  
WorldWidget is a modular Unreal Engine component library for rendering interactive widgets projected from 3D world locations onto the 2D screen. It provides a flexible architecture to manage, render, and update UI elements in real-time based on actors' positions or other spatial logic. Perfect for in-game markers, tooltips, indicators, and dynamic HUD elements.

## Features

- Project arbitrary UI widgets from 3D world locations to the screen
- Easy integration via component-based design (`WorldWidgetComponent`)
- Support for spatial logic: visibility, distance ordering, alignment, anchors, and pivots
- Real-time widget updates and redraws
- Blueprint-friendly interfaces for custom widget providers
- Extensible projection system via `WorldToScreenLocationProjector`
- Event-driven visibility and widget construction

## Installation

**Prerequisites:**

- Unreal Engine (tested with 4.x/5.x branches)
- C++ project setup

**Setup Steps:**

1. Copy all `WorldWidget` source files (`WorldWidgetCanvas.*`, `WorldWidgetProvider.*`, `WorldWidgetComponent.*`, `WorldToScreenLocationProjector.*`) into your project's Source directory.
2. Include appropriate header files in your game modules.
3. Regenerate project files and compile in Unreal Editor.

# Setup Guide

This guide walks you through setting up world-space widgets that display markers above actors in your scene.

## Prerequisites

You should already have actors in your scene that need markers displayed above them.

![[Actors.png]]

## Step 1: Create the Scene Component

Create a new scene component that will serve as the marker provider.

![[SceneComponent.png]]

## Step 2: Implement the Interface

Implement the `IWorldWidgetProvider` interface in your component class and override the required methods:

- `GetWorldLocation` - Returns the world position where the widget should be displayed
    
- `GetWidget` - Returns the widget instance to display

## Step 3: Create Your Custom Widget in The Component

Create the widget that will be displayed as the marker.

![[WidgetCreation.png]]

## Synchronizing Widget Data

Use the `NotifyWidgetConstruction` event to initialize your widget when it's added to the screen. This ensures your widget stays in sync with the component's data.

![[NotifyWidget.png]]

## Step 4: Create the Canvas Widget

Create a new widget blueprint that will serve as the canvas for all markers.

![[SelectTheClass.png]]  
![[NameYourWidget.png]]

Ensure the widget hierarchy contains a `CanvasPanel` as the root or primary container.

![[CanvasPanelIsThere.png]]

## Step 5: Configure Your Actors

Add the component to your actors and configure their marker settings.

![[AddComponent.png]]  
![[SetupNames.png]]

## Step 6: Initialize the System

Create the canvas widget and gather all provider components to display the markers. This example shows setup in the player controller, but you can implement this in any suitable class.

## Create and Add the Canvas Widget

![[CreateAndAddWidget.png]]

## Gather Components and Initialize

![[GetAllComponentsAndInitalizeWidget.png]]

## Result

Your world widgets should now be displaying correctly above your actors.

![[voilà.png]]
## API Documentation

### Core Classes

| Class                     | Description                                                      |
|---------------------------|------------------------------------------------------------------|
| `UWorldWidgetCanvas`      | Manages rendering & ordering widgets on the screen               |
| `UWorldWidgetComponent`   | Actor component as widget provider or container                  |
| `IWorldWidgetProvider`    | Interface for objects that offer widgets & locations             |
| `UWorldToScreenLocationProjector` | Handles projection math from 3D to 2D screen           |

### Selected Methods

- **UWorldWidgetCanvas**
  - `void RedrawWith(const TArray<TScriptInterface<IWorldWidgetProvider>>& Providers)`
  - `void AddUnique(const TScriptInterface<IWorldWidgetProvider>& Provider)`
  - `void Remove(const TScriptInterface<IWorldWidgetProvider>& Provider)`
  - `void Clear()`
  - `bool Contains(const TScriptInterface<IWorldWidgetProvider>& Provider)`

- **UWorldWidgetComponent**
  - `virtual UUserWidget* GetWidgetImplementation() override`
  - `virtual bool IsVisibleImplementation() override`
  - `virtual FVector GetWorldLocationImplementation() override`

- **UWorldToScreenLocationProjector**
  - `void Initialize(APlayerController* OwningPlayer)`
  - `void SetWorldLocation(FVector WorldLocation)`
  - `bool Project()`
  - `FVector2D GetLocalPositionFromViewportPosition(const FGeometry& Geometry)`

## Contributing

- Fork this repository and create a feature branch
- Write utility or widget extensions in Blueprint/C++
- Ensure code quality via Unreal's coding standards
- Submit pull requests with clear descriptions
- All major changes require discussion via issues before submission
