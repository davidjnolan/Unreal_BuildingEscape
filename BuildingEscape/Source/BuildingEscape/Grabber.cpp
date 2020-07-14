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

	GetPlayerViewpoint();
	if (!PhysicsHandle){return;} // Protecting against a nullptr
	if (PhysicsHandle->GrabbedComponent) // if Physics handle is attached
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd); // Move the object we are holding (lock to end of reach)
	}
}

//Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
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

// Updates PlayerViewPointLocation, PlayerViewPointRotation and LineTraceEnd with the player's current location
void UGrabber::GetPlayerViewpoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	
	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	if (EnableDebug)
	{
		DrawGrabberDebug();
	}
}

// Calls GetFirstPhysicsBodyInReach and attaches a Physics Handle to the component hit by the line trace
void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if(!PhysicsHandle){return;} // Protecting against a nullptr
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

// Releases Physics Handle
void UGrabber::Release()
{
	if (!PhysicsHandle){return;} // Protecting against a nullptr
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Draw debug line previewing Reach line trace
void UGrabber::DrawGrabberDebug() const
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
}

// Return the first Actor in reach with physics body
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
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
	
	if (EnableDebug && Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}