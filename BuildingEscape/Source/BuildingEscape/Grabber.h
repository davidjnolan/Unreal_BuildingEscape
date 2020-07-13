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
	void FindPhysicsHandle();
	void SetupInputComponent();
	void Grab();
	void Release();
	void GrabberDebug(FVector PlayerViewPointLocation, FVector LineTraceEnd, FHitResult Hit) const;

	FHitResult GetFirstPhysicsBodyInReach() const;
	

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPROPERTY(EditAnywhere, DisplayName="Enable Debug")
	bool EnableDebug = true;
};
