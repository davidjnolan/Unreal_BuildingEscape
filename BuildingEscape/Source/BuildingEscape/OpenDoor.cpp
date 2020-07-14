// Copyright PixelSpawn 2020

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	FindAudioComponent();
	CheckPressurePlate();
	
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
	GetWorld()->GetTimeSeconds();
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = (FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed));
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseAudioPlayed = false;
	if (!AudioComponent){return;}
	if (!OpenAudioPlayed)
	{
		AudioComponent->Play(0.f);
		OpenAudioPlayed = true;
	}
	
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = (FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed));
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenAudioPlayed = false;
	if (!AudioComponent){return;}
	if (!CloseAudioPlayed)
	{
		AudioComponent->Play(0.f);
		CloseAudioPlayed = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping Actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate){return TotalMass;} // Protecting against a nullptr
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* OverlappingActor : OverlappingActors)
	{
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio Component missing on %s"), *GetOwner()->GetName());
	}	
}

void UOpenDoor::CheckPressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no Pressure Plate set!"), *GetOwner()->GetName());
	}
}