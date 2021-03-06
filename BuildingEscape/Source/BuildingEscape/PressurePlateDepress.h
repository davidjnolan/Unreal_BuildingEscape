// Copyright PixelSpawn 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "PressurePlateDepress.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UPressurePlateDepress : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressurePlateDepress();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CheckPressurePlateVolume();
	void CheckMesh();
	void FindAudioComponent();
	void LowerPressurePlate(float DeltaTime);
	void RaisePressurePlate(float DeltaTime);
	float TotalMassOfActors() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlateVolume = nullptr;

	UPROPERTY()
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	float InitialPosition;
	float CurrentPosition;
	UPROPERTY(EditAnywhere)
	float TargetPosition = -10.f;


	float PressurePlateLastDepressed = 0.f;


	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 60.f;

	UPROPERTY(EditAnywhere)
	float PressurePlateSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	bool WillRaise = true;

	UPROPERTY(EditAnywhere)
	float PressurePlateRaiseDelay = 0.f;


	// Tracks whether the sound has been played
	bool LowerAudioPlayed = false;
	bool RaiseAudioPlayed = true;
};
