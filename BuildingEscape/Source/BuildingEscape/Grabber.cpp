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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if Physics handle is attached
		//Move the object we are holding
}

//Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{	
		// Physics Handle Found - no action required
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle found on %s"), *GetOwner()->GetName());
	}
}

// Check for Input Component and setup key bindings
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	GetFirstPhysicsBodyInReach();

	// TODO only raycast when Grabber is Pressed and see if we reach any actors with a physics body channel set

	// if we hit something, then attach the physics handle
	// TODO: attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

	// TODO remove / release physics handle
}

// Return the first Actor in reach with physics body
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get Player's Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

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
	
	if (EnableDebug)
	{
		GrabberDebug(PlayerViewPointLocation, LineTraceEnd, Hit);
	}

	return Hit;
}


// Draw debug line previewing Reach line trace and report which actors are hit
void UGrabber::GrabberDebug(FVector PlayerViewPointLocation, FVector LineTraceEnd, FHitResult Hit) const
{
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
	
	// AActor* ActorHit = Hit.GetActor();
	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *Hit.GetActor()->GetName());
	}
}