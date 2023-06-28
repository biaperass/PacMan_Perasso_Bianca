// Fill out your copyright notice in the Description page of Project Settings.


#include "FruitActor.h"

AFruitActor::AFruitActor()
{
	PrimaryActorTick.bCanEverTick = true;

	//imposto la mesh alla frutta
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	HideInGame = false;

}

void AFruitActor::DisableActor()
{

	SetActorHiddenInGame(HideInGame);


	SetActorEnableCollision(false);


	SetActorTickEnabled(false);
}

void AFruitActor::BeginPlay()
{
	Super::BeginPlay();

}


void AFruitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

