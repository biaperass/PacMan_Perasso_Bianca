// Fill out your copyright notice in the Description page of Project Settings.



// La velocità del PacMan al 100 % dovrebbe essere di 75, 75 pixel al secondo, ogni tile è di 8 pixel quindi dovrebbe essere nel range di 10 tile al secondo

// Modalità Chase e Scatter
// Velocità del PacMan 80% = 400
// Velocità dei fantasmi 75% = 375

// Modalità Spaventato
// Velocità del PacMan 90% = 450
// Velocità dei fantasmi 50% = 250


#include "PacManGameMode.h"
#include "Kismet/GameplayStatics.h"

APacManGameMode::APacManGameMode()
{

}

void APacManGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector GridPos(50.0f, 50.0f, 0);


	if (MazeGeneratorClass != nullptr)
	{
		GMaze = GetWorld()->SpawnActor<AMazeGenerator>(MazeGeneratorClass, GridPos, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}

	Blinky = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(), BlinkyClass));
	Pinky = Cast<APinky>(UGameplayStatics::GetActorOfClass(GetWorld(), PinkyClass));
	Inky = Cast<AInky>(UGameplayStatics::GetActorOfClass(GetWorld(), InkyClass));
	Clyde = Cast<AClyde>(UGameplayStatics::GetActorOfClass(GetWorld(), ClydeClass));
	PacMan = Cast<APacManPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), PacManClass));

	EnterSpawnState();
}




//gestione degli stati 
void APacManGameMode::EnterFrightenedMode()
{

	//salvo l'ultimo stato

	if (EStates == Chase)
	{
		LastState = 0;
		GetWorld()->GetTimerManager().PauseTimer(ChaseTimer);

	}

	if (EStates == Scatter)
	{
		LastState = 1;
		GetWorld()->GetTimerManager().PauseTimer(ScatterTimer);

	}

	//imposto lo sttao corrente a frightened

	EStates = Frightened;


	//inizializzoil timer
	float FrightenedTime = 6.0;
	GetWorld()->GetTimerManager().SetTimer(FrightnedTimer, this, &APacManGameMode::FrightenedExit, FrightenedTime, false);

	//imposto velocità

	PacMan->CurrentMovementSpeed = (PacMan->StandardSpeed / 100) * 90;;


	Blinky->GhostFrightenedState();

	Inky->GhostFrightenedState();

	Pinky->GhostFrightenedState();

	Clyde->GhostFrightenedState();

}

void APacManGameMode::FrightenedExit()
{

	//ritorna allo stato precedente
	if (LastState == 0)
	{
		EnterChaseMode();

	}

	if (LastState == 1)
	{

		EnterScatterMode();

	}

}

void APacManGameMode::EnterChaseMode()
{

	//in cremnta il chase counter

	Chase_Counter += 1;

	//inizializza il timer 
	float ChaseTime = 20.0;

	//After the third time it stays in Chase state

	if (Chase_Counter == 1 || Chase_Counter == 2 || Chase_Counter == 3)
	{
		GetWorld()->GetTimerManager().SetTimer(ChaseTimer, this, &APacManGameMode::ChaseExit, ChaseTime, false);
	}

	//imposto lo stato corrente a chase

	EStates = Chase;

	//imposto velocità

	PacMan->CurrentMovementSpeed = (PacMan->StandardSpeed / 100) * 80;

	Inky->ChaseScatterSpeed();

	Blinky->ChaseScatterSpeed();

	Pinky->ChaseScatterSpeed();

	Clyde->ChaseScatterSpeed();

}

void APacManGameMode::ChaseExit()
{
	EnterScatterMode();
}

void APacManGameMode::EnterScatterMode()
{

	//incremento lo scatter counter

	Scatter_Counter += 1;

	//inizializzo il timer 
	float ScatterTime;

	//le prime due volte inizializzo lo scatter a 7 secondi, poi a 5

	if (Scatter_Counter == 1 || Scatter_Counter == 2)
	{
		ScatterTime = 7.0;
	}
	else
	{
		ScatterTime = 5;
	}
	GetWorld()->GetTimerManager().SetTimer(ScatterTimer, this, &APacManGameMode::ScatterExit, ScatterTime, false);

	//imposto lo stato corrente a scatter

	EStates = Scatter;

	//imposto la velocità

	PacMan->CurrentMovementSpeed = (PacMan->StandardSpeed / 100) * 80;

	Inky->ChaseScatterSpeed();

	Blinky->ChaseScatterSpeed();

	Pinky->ChaseScatterSpeed();

	Clyde->ChaseScatterSpeed();

	//in realtà i movimento nello stato scatter sono gestiti nelle classe di ogni fantasmino 

}

void APacManGameMode::ScatterExit()
{
	EnterChaseMode();
}

void APacManGameMode::Respawn()
{
	PacMan->RespawnPacMan();

	//gestito il restart di ogni fantasmino

	Inky->GhostRestart(Inky->InkySpawn);

	Blinky->GhostRestart(Blinky->BlinkySpawn);

	Pinky->GhostRestart(Pinky->PinkySpawn);

	Clyde->GhostRestart(Clyde->ClydeSpawn);

	//decremento del numero di vte  

	PacMan->LifeCounter -= 1;

	//ritorna all'ultimo stato
	if (LastState == 0)
	{
		EnterChaseMode();

	}

	if (LastState == 1)
	{

		EnterScatterMode();

	}


}

void APacManGameMode::EnterSpawnState()
{
	Inky->IsSpawnState = true;

	Blinky->IsSpawnState = true;

	Pinky->IsSpawnState = true;

	Clyde->IsSpawnState = true;

	//salva l'ultimo stato

	if (EStates == Chase)
	{
		LastState = 0;
		GetWorld()->GetTimerManager().PauseTimer(ChaseTimer);

	}

	if (EStates == Scatter)
	{
		LastState = 1;
		GetWorld()->GetTimerManager().PauseTimer(ScatterTimer);

	}

	//inizializzo il timer 
	float SpawnTime = 2;
	GetWorld()->GetTimerManager().SetTimer(SpawnStateTimer, this, &APacManGameMode::ExitSpawnState, SpawnTime, false);

}

void APacManGameMode::ExitSpawnState()
{

	Inky->IsSpawnState = false;

	Blinky->IsSpawnState = false;

	Pinky->IsSpawnState = false;

	Clyde->IsSpawnState = false;

	Clyde->ExitSpawnStateGhost();

	//ritorna all'ultimo stato
	if (LastState == 0)
	{
		EnterChaseMode();

	}

	if (LastState == 1)
	{

		EnterScatterMode();

	}


}

