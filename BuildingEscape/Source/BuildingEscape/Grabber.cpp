// Copyright Adrien Bellon-Serre

#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();


	UE_LOG(LogTemp, Warning, TEXT("Grabber is here !"));
	
	// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error: No physics handle component found for %s"), *(GetOwner()->GetName()))
	}

	//Look for attached input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"))
		// Bind the inout axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error: No physics handle component found for %s"), *(GetOwner()->GetName()))
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("grab pressed"))
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceDirection = PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection * Reach;
	// Ray-cast out to reach distance
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// See what we hit
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}
	
}

