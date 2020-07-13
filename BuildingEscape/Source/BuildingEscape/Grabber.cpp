// Copyright PixelSpawn 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber on %s Reporting for Duty!"), *GetOwner()->GetName());
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player's Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

	// Draw a line from player showing the reach
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	DrawDebugLine(
		GetWorld(),					// World
		PlayerViewPointLocation,	// Start Location
		LineTraceEnd,				// End Location
		FColor(255, 0, 0),			// Color (red)
		false,						// Line Persistence (false - redraws on every frame)
		-0.f,						// Line Lifetime (0 - redraws on every frame)
		0,							// Depth priority (apparently this doesn't work correctly)
		2.f							// Line thickness
	);

	// Ray-cast out to a certain distance (Reach)
	// See what ray hits
}

