// Copyright PixelSpawn 2020


#include "PressurePlateDepress.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPressurePlateDepress::UPressurePlateDepress()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPressurePlateDepress::BeginPlay()
{
	Super::BeginPlay();
	CheckPressurePlateVolume();
	CheckMesh();
	FindAudioComponent();
}

// Called every frame
void UPressurePlateDepress::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlateVolume && TotalMassOfActors() > MassToOpenDoor)
	{
		LowerPressurePlate(DeltaTime);
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - PressurePlateLastDepressed > PressurePlateRaiseDelay)
		{
			RaisePressurePlate(DeltaTime);
		}
	}
	GetWorld()->GetTimeSeconds();
}


void UPressurePlateDepress::CheckPressurePlateVolume()
{
	if (!PressurePlateVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the PressurePlateDepress component on it, but no Pressure Plate set!"), *GetOwner()->GetName());
	}
}

void UPressurePlateDepress::CheckMesh()
{
	for (auto Component : GetOwner()->GetComponents())
	{
		if (Component->GetName() == "SM_Button")
		{
			Mesh = Cast<UStaticMeshComponent>(Component);
			break;
		}
	}
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("No SM_Button Static Mesh component found on %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s SM found on %s"), *Mesh->GetName(), *GetOwner()->GetName());
		InitialPosition = Mesh->GetComponentLocation().Z;
		CurrentPosition = InitialPosition;
		TargetPosition += InitialPosition;
	}	
}

void UPressurePlateDepress::LowerPressurePlate(float DeltaTime)
{
	if (!Mesh) {return;}
	CurrentPosition = (FMath::FInterpTo(CurrentPosition, TargetPosition, DeltaTime, PressurePlateSpeed));
	FVector PressurePlatePosition = Mesh->GetComponentLocation();
	PressurePlatePosition.Z = CurrentPosition;
	Mesh->SetWorldLocation(PressurePlatePosition);

	RaiseAudioPlayed = false;
	if (!AudioComponent){return;}
	if (!LowerAudioPlayed)
	{
		AudioComponent->Play(0.f);
		LowerAudioPlayed = true;
	}
}

void UPressurePlateDepress::RaisePressurePlate(float DeltaTime)
{
	if (!WillRaise) {return;}
	if (!Mesh) {return;}
	CurrentPosition = (FMath::FInterpTo(CurrentPosition, InitialPosition, DeltaTime, PressurePlateSpeed));
	FVector PressurePlatePosition = Mesh->GetComponentLocation();
	PressurePlatePosition.Z = CurrentPosition;
	Mesh->SetWorldLocation(PressurePlatePosition);

	LowerAudioPlayed = false;
	if (!AudioComponent){return;}
	if (!RaiseAudioPlayed)
	{
		AudioComponent->Play(0.f);

		RaiseAudioPlayed = true;
	}
}

float UPressurePlateDepress::TotalMassOfActors() const
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

void UPressurePlateDepress::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Audio Component missing on %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Audio Component found on %s"), *GetOwner()->GetName());
	}
	
}