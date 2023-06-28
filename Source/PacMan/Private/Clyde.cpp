// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacManGameMode.h"
#include "Math/Vector.h"

//fantasmino arancione 

AClyde::AClyde()
{
}

void AClyde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Player->PointCounter == 90)
	{

		StopMovement();
	}




}

void AClyde::SetGhostTarget()
{

	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	if (IsSpawnState)
	{
		Target = *(MazeGen->TileMap.Find(FVector2D(20, 14)));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
	}
	else if (IsEaten)
	{
		Target = *(MazeGen->TileMap.Find(ClydeSpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(ClydeSpawn);

		if (CurrentGridCoords == Target->GetNodePosition())
		{

			IsEaten = false;

			ExitRespawnState();

		}


	}

	else if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase)) {

			//calcola la distanza euclidea da pacman
			if (FVector::Dist(this->GetActorLocation(), GetPlayer()->GetActorLocation()) > 850)
			{
				Target = GetPlayerRelativeTarget();

				if (!Target)
				{
					Target = GetPlayer()->GetLastNode();

				}
			}
			else
			{
				Target = ClydeScatterNode;
			}
		}
		else
		{
			Target = ClydeScatterNode;
		}

		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));

	}

	else
	{

		SetFrightenedStateTarget();
		PossibleNode = TargetNode;
	}

	if (PossibleNode)
	{
		TargetNode = PossibleNode;
		NextNode = PossibleNode;
		LastValidInputDirection = MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords());
	}
}

void AClyde::BeginPlay()
{
	AGhostPawn::BeginPlay();

	CanMove = true;

	ClydeScatterNodeCoord = FVector2D(2, 0);

	ClydeScatterNode = *(MazeGen->TileMap.Find(ClydeScatterNodeCoord));
}
