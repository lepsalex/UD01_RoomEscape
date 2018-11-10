// © Alex Lepsa 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

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

	// Find owning actor
	Owner = GetOwner();

	// Check for trigger volume
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate (ATriggerVolume)"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	// Rotate door
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));

	// Update open time
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetMassOfActorsOnPlateInKG() >= TriggerMass) {
		// If the ActorThatOpens is in the volume...
		OpenDoor();
	}

	// Check if we need to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetMassOfActorsOnPlateInKG() {

	if (!PressurePlate) { return 0.f; }

	float TotalMass = 0.f;

	// Iterate over actors on pressure plate
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* OverlappingActor : OverlappingActors) {
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

