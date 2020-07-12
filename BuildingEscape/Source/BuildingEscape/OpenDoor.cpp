// Copyright PixelSpawn 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("Object Transform is: %s"), *GetOwner()->GetActorTransform().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Object Yaw is %f: "), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = (FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2.f));
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	// float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator OpenDoor(0.f, (FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2.f)), 0.f);
	// GetOwner()->SetActorRotation(OpenDoor);
	
	

	// FRotator CurrentRotation = GetOwner()->GetActorRotation();
	// //CurrentRotation.Yaw = 90.f;
	// FRotator OpenDoor(0.f, 90.f, 0.f);
	// GetOwner()->SetActorRotation(OpenDoor);
}

