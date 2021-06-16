#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("Physics Handler not found on Grabber.cpp of %s"), *GetOwner()->GetName());
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on Grabber.cpp of %s"), *GetOwner()->GetName());
		return;
	}

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabPressed);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->
		GetFirstPlayerController()->
		GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	const FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit;

	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->
		LineTraceSingleByObjectType(Hit,
		                            PlayerViewPointLocation,
		                            LineTraceEnd,
		                            FCollisionObjectQueryParams(ECC_PhysicsBody),
		                            TraceParams
		);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *ActorHit->GetName());

	return Hit;
}

void UGrabber::GrabPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"), *GetOwner()->GetName());
	FHitResult ActorHit = GetFirstPhysicsBodyInReach();
}

void UGrabber::GrabReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"), *GetOwner()->GetName());
}
