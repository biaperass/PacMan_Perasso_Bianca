// Fill out your copyright notice in the Description page of Project Settings.


#include "PointActor.h"

APointActor::APointActor()
{

	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	HideInGame = false;


}

void APointActor::DisableActor()
{
	SetActorHiddenInGame(HideInGame);

	SetActorEnableCollision(false);

	SetActorTickEnabled(false);
}

void APointActor::BeginPlay()
{
	Super::BeginPlay();

}

void APointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}