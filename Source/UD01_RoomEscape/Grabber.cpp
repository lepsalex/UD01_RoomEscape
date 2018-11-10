// © Alex Lepsa 2018

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT // does nothing but allows us to annotate

UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent() {
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (Input) {
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s missing compoent of type UInputComponent"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing compoent of type UPhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	// If we hit, then attach a physics handle
	FHitResult  HitResult = GetFirstPhysicsBodyInReach();
	AActor* HitActor = HitResult.GetActor();
	if (HitActor) {
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(), // needs the PrimitiveComponent inclide for intellisense to be intelligent
			FRotator(0.0f)
		);
	}
}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics is attached, move object each frame else do nothing
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetLineTracePoints().EndLocation);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	FHitResult Hit;
	LineTracePoints PlayerLineTracePoints = GetLineTracePoints();

	/// Line-trace (raycast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLineTracePoints.StartLocation,
		PlayerLineTracePoints.EndLocation,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);

	return Hit;
}

UGrabber::LineTracePoints  UGrabber::GetLineTracePoints() {
	LineTracePoints PlayerLineTracePoints;
	FRotator PlayerVPRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLineTracePoints.StartLocation,
		OUT PlayerVPRotation
	);

	PlayerLineTracePoints.EndLocation = PlayerLineTracePoints.StartLocation + (PlayerVPRotation.Vector() * Reach);

	return PlayerLineTracePoints;
}
