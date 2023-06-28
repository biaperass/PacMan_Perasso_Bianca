// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostPawn.h"
#include "Kismet/GameplayStatics.h"
#include "PacManPawn.h"
#include "PacManGameMode.h"

AGhostPawn::AGhostPawn()
{

	PrimaryActorTick.bCanEverTick = true;


	//Mesh nella frightened state 
	StaticMeshBlue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blue Mesh"));
	StaticMeshWhite = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("White Mesh"));

	StaticMeshBlue->SetupAttachment(Collider);
	StaticMeshWhite->SetupAttachment(Collider);

	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);


	//tempo per alternare blu e bianco
	WhiteTime = 1.0;
	BlueTime = 1.0;
	FlashCounter = 0;

	IsEaten = false;
	IsSpawnState = false;
	IsRespawn = false;
}

void AGhostPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];
	Player = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacManPawn::StaticClass()));

}

void AGhostPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PacMan = Cast<APacManPawn>(OtherActor);
	if (PacMan)
	{
		//quando pacman mangia un fantasmino in frightened mode
		if (GameMode->EStates == Frightened)
		{
			PacMan->Eat(this);
			//PacMan->PointCounterUI += 200;

			APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
			APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

			EnterNode1->EIsLegal = Walkable;
			EnterNode2->EIsLegal = Walkable;
		}
		else
		{
			//se non si trova in frightened mode allora pacman muore e viene rispawnato
			GameMode->Respawn();

		}
	}
}

void AGhostPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void AGhostPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

APacManPawn* AGhostPawn::GetPlayer() const
{

	return Player;

}

void AGhostPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

APacManNode* AGhostPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();;
}

void AGhostPawn::SetGhostTarget()
{

	const APacManNode* Target = GetPlayerRelativeTarget();

	if (!Target)
	{
		Target = GetPlayer()->GetLastNode();
	}

	APacManNode* PossibleNode = MazeGen->ShortestNodeToTarget(this->GetLastNodeCoords(), Target->GetNodePosition(), -(this->GetLastValidDirection()));


	if (PossibleNode)
	{
		this->SetNextNodeByDir(MazeGen->GetThreeDOfTwoDVector(PossibleNode->GetNodePosition() - this->GetLastNodeCoords()), true);
	}
}


void AGhostPawn::TeleportToGhostBase()
{
	const FVector GhostBase(1750.0f, 1450.0f, 51.0f);

	CurrentGridCoords = FVector2D(17, 14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(17, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(17, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(GhostBase);
}

void AGhostPawn::GhostFrightenedState()
{
	//in frightened mode il fantasmino va a 50% della sua velocità normale
	FrightenedSpeed();

	//Make Ghosts Flash
	MeshHandler();

	//inverte la direzione del fantasmino
	InvertDirection();


}

void AGhostPawn::FrightenedSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 50;
}

void AGhostPawn::ChaseScatterSpeed()
{
	this->CurrentMovementSpeed = (StandardSpeed / 100) * 75;
}

void AGhostPawn::InvertDirection()
{
	LastValidInputDirection = LastValidInputDirection * (-1);
}

void AGhostPawn::MeshHandler()
{
	StaticMeshBlue->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
}

void AGhostPawn::MeshWhite()
{
	StaticMeshWhite->SetVisibility(true);

	GetWorld()->GetTimerManager().SetTimer(WhiteMeshTimer, this, &AGhostPawn::MeshBlue, WhiteTime, false);
}

void AGhostPawn::MeshBlue()
{
	//incremento il flashCounter 
	FlashCounter += 1;
	//imposto la mesh blu
	StaticMeshWhite->SetVisibility(false);

	if (FlashCounter < 3)
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::MeshWhite, BlueTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(BlueMeshTimer, this, &AGhostPawn::ResetMesh, BlueTime, false);
	}
}

void AGhostPawn::ResetMesh()
{
	//disabilito la mesh
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	//resetto il counter
	FlashCounter = 0;
}

void AGhostPawn::SetFrightenedStateTarget()
{
	float RandomNumber = Stream.FRandRange(0, 100);

	if (RandomNumber <= 25)
	{
		DirCasualVector = FVector(1, 0, 0);
	}
	else if (RandomNumber > 25 && RandomNumber <= 50)
	{
		DirCasualVector = FVector(0, 1, 0);
	}
	else if (RandomNumber > 50 && RandomNumber <= 75)
	{
		DirCasualVector = FVector(-1, 0, 0);
	}
	else if (RandomNumber > 75 && RandomNumber <= 100)
	{
		DirCasualVector = FVector(0, -1, 0);
	}

	RandomNodeCord = CurrentGridCoords + MazeGen->GetTwoDOfVector(DirCasualVector);
	RandomNode = *(MazeGen->TileMap.Find(RandomNodeCord));

	if (MazeGen->IsNodeValidForWalk(RandomNode))
	{
		TargetNode = RandomNode;
	}
}



void AGhostPawn::RespawnGhost(FVector2D RespawnNode)
{
	StaticMesh->SetVisibility(false);
	StaticMeshBlue->SetVisibility(false);
	StaticMeshWhite->SetVisibility(false);

	this->CurrentMovementSpeed = 1200;

	TargetNode = *(MazeGen->TileMap.Find(RespawnNode));

}

void AGhostPawn::ExitRespawnState()
{
	StaticMesh->SetVisibility(true);
	StopMovement();
	this->ChaseScatterSpeed();
}

void AGhostPawn::StopMovement()
{
	CanMove = false;

	//il fantasmino torna alla base
	this->IsSpawnState = true;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::MoveAgain, 1.0f, false);

}

void AGhostPawn::MoveAgain()
{
	CanMove = true;

	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

	EnterNode1->EIsLegal = Walkable;
	EnterNode2->EIsLegal = Walkable;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::ExitSpawnStateGhost, 2.0f, false);
}


void AGhostPawn::ExitSpawnStateGhost()
{

	//Make house nodes enter not walkable (19,13) (19,14)

	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

	EnterNode1->EIsLegal = NotWalkable;
	EnterNode2->EIsLegal = NotWalkable;

	this->IsSpawnState = false;
}

void AGhostPawn::GhostRestart(FVector2D SpawnLocation)
{
	FVector SpawnCoord = MazeGen->GetThreeDOfTwoDVector(SpawnLocation);
	SpawnCoord.X = SpawnCoord.X * 100 + 50;
	SpawnCoord.Y = SpawnCoord.Y * 100 + 50;
	SpawnCoord.Z = 1.0;
	this->TeleportPawn(SpawnCoord, SpawnLocation);
	this->IsSpawnState = true;

	APacManNode* EnterNode1 = *(MazeGen->TileMap.Find(FVector2D(19, 13)));
	APacManNode* EnterNode2 = *(MazeGen->TileMap.Find(FVector2D(19, 14)));

	EnterNode1->EIsLegal = Walkable;
	EnterNode2->EIsLegal = Walkable;

	GetWorld()->GetTimerManager().SetTimer(GhostRestartTimer, this, &AGhostPawn::ExitSpawnStateGhost, 1.0f, false);

}

void AGhostPawn::CheckGateWalkability()
{

}
