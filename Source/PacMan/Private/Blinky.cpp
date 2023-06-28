// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"
#include "PacManGameMode.h"

//fantasmino rosso 

ABlinky::ABlinky()
{
	IsElroy = false;
}

APacManNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

void ABlinky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	BlinkyScatterNodeCoord = FVector2D(35, 25);

	BlinkyScatterNode = *(MazeGen->TileMap.Find(BlinkyScatterNodeCoord));
}

void ABlinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//There are 234 points, enabled when there are 20 points left 
	if (Player->PointCounter == 214 || Player->PointCounter == 224)
	{
		ElroyEnter();
	}
	CheckGateWalkability();
}

void ABlinky::SetGhostTarget()
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
		Target = *(MazeGen->TileMap.Find(BlinkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(BlinkySpawn);

		if (CurrentGridCoords == Target->GetNodePosition())
		{
			//When respawn is reached reset the ghost 

			IsEaten = false;
			//Get out of respwan state
			ExitRespawnState();

		}

	}

	else if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase || IsElroy)) {

			Target = GetPlayerRelativeTarget();

			if (!Target)
			{
				Target = GetPlayer()->GetLastNode();
			}
		}
		else
		{
			Target = BlinkyScatterNode;
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


void ABlinky::ElroyEnter()
{

	// When Blinky is Elroy he set his speed to the same as Pacman the first time 
	//The second time Elroy goes faster than PacMan

	IsElroy = true;

	if (Player->PointCounter == 214)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("EELLLLRROOOOYYY 320")));
		CurrentMovementSpeed = (StandardSpeed / 100) * 80;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("EELLLLRROOOOYYY 400")));
		CurrentMovementSpeed = (StandardSpeed / 100) * 85;
	}

}


