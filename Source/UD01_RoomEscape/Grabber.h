// © Alex Lepsa 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UD01_ROOMESCAPE_API UGrabber : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f;

	struct LineTracePoints {
		FVector StartLocation = FVector(0.0f);
		FVector EndLocation = FVector(0.0f);
	};

	UInputComponent* Input = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	void FindPhysicsHandleComponent();
	void SetupInputComponent();

	// Grab what is in reach of raycast
	void Grab();

	// Called when grab key is released
	void Release();

	// Returns first hit of a physics body
	FHitResult GetFirstPhysicsBodyInReach();

	LineTracePoints GetLineTracePoints();
};
