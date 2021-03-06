// Copyright PixelSpawn 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
// Functions
	void FindPhysicsHandle();
	void SetupInputComponent();
	void GetPlayerViewpoint();
	void Grab();
	void Release();
	void DrawGrabberDebug() const;

	FHitResult GetFirstPhysicsBodyInReach() const;
	
	
// Member Variables
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEnd;

	UPROPERTY(EditAnywhere)
	float Reach = 150.f;

	UPROPERTY(EditAnywhere, DisplayName="Enable Debug")
	bool EnableDebug = false;
};
