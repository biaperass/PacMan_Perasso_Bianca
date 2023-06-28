// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManNode.h"

APacManNode::APacManNode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APacManNode::BeginPlay()
{
	Super::BeginPlay();

}

void APacManNode::SetNodePosition(const double InX, const double InY)
{
	return NodeGridPosition.Set(InX, InY);
}

FVector2D APacManNode::GetNodePosition() const
{
	return NodeGridPosition;
}

FVector APacManNode::GetNodeCoordinates()
{
	return NodeCoordinatesPosition;
}

void APacManNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}