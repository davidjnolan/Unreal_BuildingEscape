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
	CheckPressurePlateVolume();
	
	InitialYaw = GetOwner()->GetActorLocation().Z;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	InitialPosition = GetOwner()->GetActorLocation().Z;
	CurrentPosition = InitialPosition;
	TargetPosition += InitialPosition;
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("DeltaTime = %f"), DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), InitialYaw, CurrentYaw, OpenAngle);

	if (PressurePlateVolume && TotalMassOfActors() > MassToOpenDoor)
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
	CurrentPosition = (FMath::FInterpTo(CurrentPosition, TargetPosition, DeltaTime, DoorOpenSpeed));
	FVector DoorPosition = GetOwner()->GetActorLocation();
	DoorPosition.Z = CurrentPosition;
	GetOwner()->SetActorLocation(DoorPosition);

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
	if (!WillClose) {return;}

	CurrentPosition = (FMath::FInterpTo(CurrentPosition, InitialPosition, DeltaTime, DoorCloseSpeed));
	FVector DoorPosition = GetOwner()->GetActorLocation();
	DoorPosition.Z = CurrentPosition;
	GetOwner()->SetActorLocation(DoorPosition);

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
	if (!PressurePlateVolume){return TotalMass;} // Protecting against a nullptr
	PressurePlateVolume->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->GetName().Contains("PressurePlate"))
		{
			continue; // ignore PressurePlate in mass calculation
		}
		else
		{
			TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
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

void UOpenDoor::CheckPressurePlateVolume()
{
	if(!PressurePlateVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no Pressure Plate set!"), *GetOwner()->GetName());
	}
}