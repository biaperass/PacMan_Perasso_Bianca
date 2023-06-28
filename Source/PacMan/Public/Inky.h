// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Inky.generated.h"

//fantasmino azzurro

UCLASS()
class PACMAN_API AInky : public AGhostPawn
{
	GENERATED_BODY()

public:

	AInky();

	virtual void Tick(float DeltaTime) override;

	virtual void SetGhostTarget() override;

	virtual void BeginPlay() override;

	FVector2D InkySpawn = FVector2D(17, 12);

private:

	FVector2D InkyScatterNodeCoord;

	APacManNode* InkyScatterNode;


};
