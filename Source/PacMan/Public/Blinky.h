// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostPawn.h"
#include "Blinky.generated.h"

//fantasmino rosso

UCLASS()
class PACMAN_API ABlinky : public AGhostPawn
{
	GENERATED_BODY()

private:

	FVector2D BlinkyScatterNodeCoord;

	APacManNode* BlinkyScatterNode;


public:
	FVector2D BlinkySpawn = FVector2D(17, 15);

	ABlinky();

	virtual void BeginPlay() override;

	virtual void SetGhostTarget() override;

	virtual APacManNode* GetPlayerRelativeTarget() override;

	virtual void Tick(float DeltaTime) override;


	void ElroyEnter();


	UPROPERTY(EditAnywhere)
		bool IsElroy;

};
