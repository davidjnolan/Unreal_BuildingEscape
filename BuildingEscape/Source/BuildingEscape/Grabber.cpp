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

	//Checking for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{	
		// Physics Handle Found - no action required
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle found on %s"), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		UE_LOG(LogTemp, Warning, TEXT("Input Component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component found on %s"), *GetOwner()->GetName());
	}
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
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); 	// Tags to check, whether we want complex tags, Actors to ignore
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,															// Hit result Out Param
		PlayerViewPointLocation,											// Trace Start Location
		LineTraceEnd,														// Trace End Location
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),	// Collision Object query - what type of object do we want to hit? - PhysicsBody in this case
		TraceParams															// Trace Parameters (see above)
	);
	
	// See what ray hits
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *ActorHit->GetName());
	}
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
}