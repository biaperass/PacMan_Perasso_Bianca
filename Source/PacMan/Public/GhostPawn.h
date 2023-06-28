// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazePawn.h"
#include "GhostPawn.generated.h"




UCLASS()
class PACMAN_API AGhostPawn : public AMazePawn
{
	GENERATED_BODY()
public:
	AGhostPawn();

protected:
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;

private:
	

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		APacManPawn* GetPlayer() const;

	UFUNCTION()
		void SetSpeed(float Speed);

	UFUNCTION()
		virtual APacManNode* GetPlayerRelativeTarget();

	UPROPERTY(VisibleAnywhere)
		class APacManPawn* Player;


	virtual void SetGhostTarget();


	void TeleportToGhostBase();

	//cambio di mesh nella frightened mode
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshBlue;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshWhite;

	//funzione che gestisce il frightened mode 

	void GhostFrightenedState();

	void FrightenedSpeed();
	void ChaseScatterSpeed();

	//funzione per invertire direzione 

	void InvertDirection();

	//funzione e parametri per impostare il fantasmino blu 

	void MeshHandler();
	void MeshWhite();
	void MeshBlue();
	void ResetMesh();

	FTimerHandle WhiteMeshTimer;
	FTimerHandle BlueMeshTimer;
	float WhiteTime;
	float BlueTime;
	float FlashCounter;

	//funzione per impostare random target

	void SetFrightenedStateTarget();

	FRandomStream Stream;

	FVector DirCasualVector;

	FVector2D RandomNodeCord;

	APacManNode* RandomNode;


	//variabili per vedere se il fantasmino è stato mangiato e per il respawn

	bool IsEaten;

	void RespawnGhost(FVector2D RespawnNode);

	void ExitRespawnState();

	FTimerHandle GhostRestartTimer;

	void StopMovement();
	void MoveAgain();

	//spawn 

	bool IsSpawnState;

	bool IsRespawn;

	void ExitSpawnStateGhost();

	//stato in cui il pacman è mangiato 

	void GhostRestart(FVector2D SpawnLocation);

	void CheckGateWalkability();
};


