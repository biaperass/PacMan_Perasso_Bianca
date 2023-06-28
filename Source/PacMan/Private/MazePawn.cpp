// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"
#include "PacManGameMode.h"\


AMazePawn::AMazePawn()
{

	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Collider);


	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;

}

void AMazePawn::SetVerticalInput(float AxisValue)
{
}

void AMazePawn::SetHorizontalInput(float AxisValue)
{
}

void AMazePawn::BeginPlay()
{

	Super::BeginPlay();
	GameMode = (APacManGameMode*)(GetWorld()->GetAuthGameMode());
	MazeGen = GameMode->GMaze;
	CurrentMovementSpeed = StandardSpeed;
	CanMove = true;

	//necessario per gestire le collisioni
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AMazePawn::OnOverlapBegin);
}

void AMazePawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove)
	{
		HandleMovement();
	}

}

void AMazePawn::HandleMovement()
{
	MoveToCurrentTargetNode();
	if (TargetNode == nullptr)
	{
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
}

void AMazePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMazePawn::TeleportPawn(FVector Location, FVector2D SpawnNode)
{

	CurrentGridCoords = MazeGen->GetTwoDOfVector(Location);

	LastNode = *(MazeGen->TileMap.Find(SpawnNode));

	SetNextNode(*(MazeGen->TileMap.Find(SpawnNode)));

	SetTargetNode(NextNode);

	SetActorLocation(Location);

}



void AMazePawn::MoveToCurrentTargetNode()
{
	if (TargetNode == nullptr) return;
	const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));
	if (Dist <= AcceptedDistance)
	{
		OnNodeReached();
		return;
	}
	// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
	const FVector2D StartVector = MazeGen->GetTwoDOfVector(GetActorLocation());
	const FVector2D EndVector = MazeGen->GetTwoDOfVector(TargetNode->GetActorLocation());
	const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
	const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
	SetActorLocation(Location);
}

void AMazePawn::OnNodeReached()
{
	CurrentGridCoords = TargetNode->GetNodePosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);

	//implementazione del nodo di teletrasporto
	const FVector2D Node_check = LastNode->GetNodePosition();

	//teletrasporto nella posizione (17,0) se pacman è in (17,27) and va verso destra 
	if (Node_check == FVector2D(17, 26) && LastValidInputDirection == FVector(0, 1, 0))
	{

		const FVector Location(1750.0f, 150.0f, GetActorLocation().Z);

		CurrentGridCoords = FVector2D(17, 1);

		LastNode = *(MazeGen->TileMap.Find(FVector2D(17, 1)));

		SetNextNode(*(MazeGen->TileMap.Find(FVector2D(17, 1))));

		SetTargetNode(NextNode);

		SetActorLocation(Location);
	}

	//teletrasporto nella posizione (14,27)se pacman è in (14,0) e va verso sinistra
	if (Node_check == FVector2D(17, 1) && LastValidInputDirection == FVector(0, -1, 0))
	{

		const FVector Location(1750.0f, 2650.0f, GetActorLocation().Z);

		CurrentGridCoords = FVector2D(17, 26);

		LastNode = *(MazeGen->TileMap.Find(FVector2D(17, 26)));

		SetNextNode(*(MazeGen->TileMap.Find(FVector2D(17, 26))));

		SetTargetNode(NextNode);

		SetActorLocation(Location);
	}
}

void AMazePawn::SetTargetNode(APacManNode* Node)
{
	TargetNode = Node;
}

void AMazePawn::SetNextNode(APacManNode* Node)
{
	NextNode = Node;
}

void AMazePawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = MazeGen->GetNextNode(CurrentGridCoords, Dir);
	if (MazeGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
	}
}

void AMazePawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetNodePosition() : LastNode->GetNodePosition();
	const auto Node = GameMode->GMaze->GetNextNode(Coords, InputDir);
	if (GameMode->GMaze->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

APacManNode* AMazePawn::GetLastNode() const
{
	return LastNode;
}

APacManNode* AMazePawn::GetTargetNode() const
{
	return TargetNode;
}

FVector AMazePawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

FVector2D AMazePawn::GetLastNodeCoords() const
{
	if (LastNode)
	{
		return LastNode->GetNodePosition();
	}
	return FVector2D(0, 0);
}


FVector2D AMazePawn::GetTargetNodeCoords() const
{
	if (TargetNode)
	{
		return TargetNode->GetNodePosition();
	}
	return FVector2D::ZeroVector;
}

void AMazePawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}