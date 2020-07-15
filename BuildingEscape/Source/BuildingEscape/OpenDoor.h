// Copyright PixelSpawn 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void FindAudioComponent();
	void CheckPressurePlateVolume();

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	
private:

	float InitialYaw;
	float CurrentYaw;
	float DoorLastOpened = 0.f;

	// Tracks whether the sound has been played
	bool OpenAudioPlayed = false;
	bool CloseAudioPlayed = true;
	
	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 60.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 4.f;

	UPROPERTY(EditAnywhere)
	bool WillClose = true;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlateVolume = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
