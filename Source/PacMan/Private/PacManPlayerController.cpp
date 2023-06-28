#include "PacManPlayerController.h"

APacManPlayerController::APacManPlayerController()
{
	bShowMouseCursor = true;
}

void APacManPlayerController::BeginPlay()
{

}

void APacManPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// azioni di movimento sulla griglia 
	InputComponent->BindAxis(TEXT("MoveY"), this, &APacManPlayerController::SetVertical);
	InputComponent->BindAxis(TEXT("MoveX"), this, &APacManPlayerController::SetHorizontal);
}

void APacManPlayerController::SetHorizontal(float Amount)
{
	const auto P = Cast<AMazePawn>(GetPawn());
	if (P)
	{
		// Imposta l'input orizzontale del giocatore PacMan
		P->SetHorizontalInput(Amount);
	}
}

void APacManPlayerController::SetVertical(float Amount)
{
	const auto P = Cast<AMazePawn>(GetPawn());
	if (P)
	{
		// Imposta l'input verticale del giocatore PacMan
		P->SetVerticalInput(Amount);
	}
}
