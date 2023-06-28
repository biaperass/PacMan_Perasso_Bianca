// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"
#include "PacManGameMode.h"

//fantasmino rosa 

APinky::APinky()
{
}

void APinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckGateWalkability();

}

void APinky::SetGhostTarget()
{
	APacManNode* Target = nullptr;

	APacManNode* PossibleNode = nullptr;

	FVector2D Tmp_Coord = FVector2D(0, 0);

	if (IsSpawnState)
	{
		Target = *(MazeGen->TileMap.Find(FVector2D(20, 14)));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
	}
	else if (IsEaten)
	{
		Target = *(MazeGen->TileMap.Find(PinkySpawn));
		PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));
		RespawnGhost(PinkySpawn);

		if (CurrentGridCoords == Target->GetNodePosition())
		{

			IsEaten = false;

			ExitRespawnState();

		}

	}
	else if (GameMode->EStates != Frightened)
	{
		if ((GameMode->EStates == Chase)) {



			if (Player->GetLastValidDirection() == FVector(0, 1, 0))
			{
				//FVector con posizione di pacman, poi sommo la quantità desiderata, infine cerco nella tile map il valore 

				Tmp_Coord = (Player->GetLastNodeCoords() + FVector2D(4, 0));
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
				Target = *(MazeGen->TileMap.Find(Tmp_Coord));
			}
			else if (Player->GetLastValidDirection() == FVector(0, -1, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(-4, 0);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
				Target = *(MazeGen->TileMap.Find(Tmp_Coord));

			}
			else if (Player->GetLastValidDirection() == FVector(1, 0, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(0, 4);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
				Target = *(MazeGen->TileMap.Find(Tmp_Coord));

			}
			else if (Player->GetLastValidDirection() == FVector(-1, 0, 0))
			{
				Tmp_Coord = Player->GetLastNodeCoords() + FVector2D(0, -4);
				Tmp_Coord.X = FMath::Clamp(Tmp_Coord.X, 0, 36);
				Tmp_Coord.Y = FMath::Clamp(Tmp_Coord.Y, 0, 27);
				Target = *(MazeGen->TileMap.Find(Tmp_Coord));

			}
			else
			{
				Target = GetPlayerRelativeTarget();

				if (!Target)
				{
					Target = GetPlayer()->GetLastNode();
				}
			}

		}


		else
		{
			Target = PinkyScatterNode;
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

void APinky::BeginPlay()
{
	AGhostPawn::BeginPlay();

	PinkyScatterNodeCoord = FVector2D(34, 2);

	PinkyScatterNode = *(MazeGen->TileMap.Find(PinkyScatterNodeCoord));
}
