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

<img width="1087" height="644" alt="Actors" src="https://github.com/user-attachments/assets/e57ee066-fe75-4e75-8a6a-a8c2495a89d2" />


## Step 1: Create the Scene Component

Create a new scene component that will serve as the marker provider.

<img width="1109" height="457" alt="SceneComponent" src="https://github.com/user-attachments/assets/8767c36a-5b0e-429c-b7d3-ade8580140a9" />

## Step 2: Implement the Interface

Implement the `IWorldWidgetProvider` interface in your component class and override the required methods:

- `GetWorldLocation` - Returns the world position where the widget should be displayed
    
- `GetWidget` - Returns the widget instance to display

## Step 3: Create Your Custom Widget in The Component

Create the widget that will be displayed as the marker.

<img width="780" height="308" alt="WidgetCreation" src="https://github.com/user-attachments/assets/b4295b67-5e3f-4ac0-bdef-b9477519787c" />

## Synchronizing Widget Data

Use the `NotifyWidgetConstruction` event to initialize your widget when it's added to the screen. This ensures your widget stays in sync with the component's data.

<img width="1007" height="350" alt="NotifyWidget" src="https://github.com/user-attachments/assets/53159539-48a5-4721-85a9-116d9deb1028" />

## Step 4: Create the Canvas Widget

Create a new widget blueprint that will serve as the canvas for all markers.

<img width="631" height="334" alt="SelectTheClass" src="https://github.com/user-attachments/assets/988398ee-c485-4610-b0b9-ff90066aa95d" />
<img width="1031" height="444" alt="NameYourWidget" src="https://github.com/user-attachments/assets/da917cb1-5649-47e6-b7d3-618d75690013" />

Ensure the widget hierarchy contains a `CanvasPanel` as the root or primary container.

<img width="1202" height="755" alt="CanvasPanelIsThere" src="https://github.com/user-attachments/assets/0ae11440-5a66-418b-afed-7bfada62c518" />

## Step 5: Configure Your Actors

Add the component to your actors and configure their marker settings.

<img width="1673" height="592" alt="AddComponent" src="https://github.com/user-attachments/assets/9a341717-ae96-430c-acb3-e4eb5e7751ae" />
<img width="1490" height="625" alt="SetupNames" src="https://github.com/user-attachments/assets/056314af-1c41-4d16-90ec-49be812df8f5" />

## Step 6: Initialize the System

Create the canvas widget and gather all provider components to display the markers. This example shows setup in the player controller, but you can implement this in any suitable class.

## Create and Add the Canvas Widget

<img width="958" height="316" alt="CreateAndAddWidget" src="https://github.com/user-attachments/assets/098bfbb5-dceb-41ad-a75e-5c09b699f34e" />

## Gather Components and Initialize

<img width="1261" height="331" alt="GetAllComponentsAndInitalizeWidget" src="https://github.com/user-attachments/assets/e6e0e665-a950-40a6-94a7-4b0bd9e305d6" />

## Result

Your world widgets should now be displaying correctly above your actors.

<img width="1278" height="711" alt="voilà" src="https://github.com/user-attachments/assets/063e3d14-d04a-4746-a0ff-cdc11594e718" />

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
