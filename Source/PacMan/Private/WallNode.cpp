// Fill out your copyright notice in the Description page of Project Settings.


#include "WallNode.h"

//imposto le pareti not walkable, cos� pacman non pu� passarci sopra

AWallNode::AWallNode()
{
	EIsLegal = NotWalkable;
}