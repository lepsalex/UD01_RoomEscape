// © Alex Lepsa 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

	// Find owning actor
	Owner = GetOwner();

	// Check for trigger volume
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate (ATriggerVolume)"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetMassOfActorsOnPlateInKG() >= TriggerMass) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
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

