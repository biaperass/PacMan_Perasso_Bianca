#include "PacManPawn.h"
#include "PacManGameMode.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"

APacManPawn::APacManPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);

	CurrentGridCoords = FVector2D(8, 14);

	NormalMovementSpeed = (StandardSpeed / 100) * 80;

	// il frutto dovrebbe apparire vicino al centro del labirinto (da specifiche)
	minX = 6;
	maxX = 21;

	minY = 11;
	maxY = 26;

	Spawnable = false;
}

void APacManPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector2D StartNode = MazeGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = MazeGen->TileMap[StartNode];

	LifeCounter = 3.0;
	PointCounter = 0.0;
	PointCounterUI = 0.0;
}

void APacManPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove finché il nodo successivo è walkable
	if (!TargetNode && !NextNode)
	{
		if (MazeGen->IsNodeValidForWalk(MazeGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}

void APacManPawn::SetTargetNode(APacManNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacManPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Point = Cast<APointActor>(OtherActor);
	const auto PowerPoint = Cast<APowerPoint>(OtherActor);
	const auto FruitCast = Cast<AFruitActor>(OtherActor);

	if (Point)
	{

		Point->HideInGame = true;
		Point->DisableActor();

		// incrementa il punteggio quando viene mangiato un punto normale
		PointCounter += 1;
		PointCounterUI += 10;

	}

	if (PowerPoint)
	{
		PowerPoint->HideInGame = true;
		PowerPoint->DisableActor();
		PowerPoint->IsEaten = true;

		PointCounterUI += 150;

		// Quando viene mangiato un punto speciale, entra in modalità frightened
		GameMode->EnterFrightenedMode();
	}

	if (FruitCast)
	{
		Fruit->HideInGame = true;
		Fruit->DisableActor();

		PointCounterUI += 100;
	}
}

void APacManPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = MazeGen->GetNextNode(CurrentGridCoords, Dir);
	if (MazeGen->IsNodeValidForWalkPacMan(Node))
	{
		SetTargetNode(Node);
	}
}

void APacManPawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetNodePosition() : LastNode->GetNodePosition();
	const auto Node = GameMode->GMaze->GetNextNode(Coords, InputDir);
	if (GameMode->GMaze->IsNodeValidForWalkPacMan(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

void APacManPawn::Eat(AGhostPawn* Ghost)
{
	// quando un fantasma viene mangiato, viene portato alla ghostbase
	UGameplayStatics::PlaySound2D(this, eat_ghost);
	const FVector GhostBase = FVector(1450.0f, 1250.0f, 51.0f);
	Ghost->IsEaten = true;
}

void APacManPawn::PointEatenExit()
{

}

void APacManPawn::RespawnPacMan()
{
	// decrementa il numero delle vite

	// riporta pacman al punto di spawn iniziale

	const FVector Spawn = FVector(850.0f, 1450.0f, GetActorLocation().Z);

	CurrentGridCoords = FVector2D(8, 14);

	LastNode = *(MazeGen->TileMap.Find(FVector2D(8, 14)));

	SetNextNode(*(MazeGen->TileMap.Find(FVector2D(8, 14))));

	SetTargetNode(NextNode);

	SetActorLocation(Spawn);
}

void APacManPawn::SpawnFruit()
{
	while (!Spawnable)
	{
		int32 Randx = int32(FMath::FRandRange(minX, maxX));
		int32 Randy = int32(FMath::FRandRange(minY, maxY));

		FVector2D FruitCoord = FVector2D(Randx, Randy);

		APacManNode* SpawnFruitNode = *(MazeGen->TileMap.Find(FruitCoord));

		FVector Position = MazeGen->GetRelativeLocationByXYPosition(Randx, Randy);

		Spawnable = MazeGen->IsNodeValidForWalkPacMan(SpawnFruitNode);

		Position.X += 50;
		Position.Y += 50;

		if (Spawnable)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FRUTTA SPAWNATA")));
			Fruit = GetWorld()->SpawnActor<AFruitActor>(FruitActor, Position, FRotator::ZeroRotator);
		}
	}
}

void APacManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PointCounter == 70 || PointCounter == 170)
	{
		SpawnFruit();
	}
}

void APacManPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection, true);
}

void APacManPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection, true);
}

void APacManPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// input delle azioni qui
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPawn::SetHorizontalInput);
}
