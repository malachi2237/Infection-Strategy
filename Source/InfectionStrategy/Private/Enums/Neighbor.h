// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** Enum codifying types of neighbors by their position relative to another */
UENUM()
enum class ENeighbor : uint8
{
	Up,
	Down,
	Left,
	Right,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};
