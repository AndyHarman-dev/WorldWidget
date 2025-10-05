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

## Tech Stack

- **Language:** C++
- **Framework:** Unreal Engine (Blueprint & C++ API)
- **Modules:** UMG (Unreal Motion Graphics), Slate, CoreMinimal

Dependencies (from includes):

- UMG Widgets: `BlueprintUserWidget`, `CanvasPanel`, `Border`
- Slate UI: `SlateSGameLayerManager`, `EasyLog`
- Core Unreal: `CoreMinimal.h`, `SceneComponent.h`
- Project-specific: `WorldWidgetCanvas.h`, `WorldWidgetProvider.h`,`WorldToScreenLocationProjector.h`

## Installation

**Prerequisites:**

- Unreal Engine (tested with 4.x/5.x branches)
- C++ project setup

**Setup Steps:**

1. Copy all `WorldWidget` source files (`WorldWidgetCanvas.*`, `WorldWidgetProvider.*`, `WorldWidgetComponent.*`, `WorldToScreenLocationProjector.*`) into your project's Source directory.
2. Include appropriate header files in your game modules.
3. Regenerate project files and compile in Unreal Editor.

## Usage

### Example: Add a WorldWidget to an Actor

```cpp
// In your actor header
#include "WorldWidgetComponent.h"

// In constructor
UWorldWidgetComponent* WidgetComp = CreateDefaultSubobject<UWorldWidgetComponent>(TEXT("WorldWidget"));
WidgetComp->WidgetClass = MyCustomUserWidgetClass; // Your UUserWidget Blueprint/Class
WidgetComp->Pivot = FVector2D(0.5f, 0.5f); // Center pivot
```

### Interactive Widget Projection

```cpp
// Redraw widgets for provider actors
TArray<TScriptInterface<IWorldWidgetProvider>> WidgetProviders = ...;
WorldWidgetCanvas->RedrawWith(WidgetProviders);
```

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
