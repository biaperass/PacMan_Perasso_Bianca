// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Pinky.generated.h"

//fantasmino rosa

UCLASS()
class PACMAN_API APinky : public AGhostPawn
{
	GENERATED_BODY()

public:

	APinky();

	virtual void Tick(float DeltaTime) override;


	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

	FVector2D PinkySpawn = FVector2D(17, 13);
private:

	FVector2D PinkyScatterNodeCoord;

	APacManNode* PinkyScatterNode;


};
