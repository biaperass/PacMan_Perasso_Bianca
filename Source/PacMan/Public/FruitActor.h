// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FruitActor.generated.h"

UCLASS()
class PACMAN_API AFruitActor : public AActor
{
	GENERATED_BODY()

public:

	AFruitActor();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Disable")
		bool HideInGame;

	UFUNCTION(BlueprintCallable, Category = "Disable")
		void DisableActor();
protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};
