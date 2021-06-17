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

void UGrabber::TickComponent(const float DeltaTime, const ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;

	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->
		LineTraceSingleByObjectType(Hit,
		                            GetPlayerLocation(),
		                            GetPlayerReach(),
		                            FCollisionObjectQueryParams(ECC_PhysicsBody),
		                            TraceParams
		);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *ActorHit->GetName());

	return Hit;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->
		GetFirstPlayerController()->
		GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerLocation() const
{
	return GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}

void UGrabber::GrabPressed()
{
	const FHitResult ActorHit = GetFirstPhysicsBodyInReach();
	if (ActorHit.GetActor())
		PhysicsHandle->GrabComponentAtLocation(ActorHit.GetComponent(), NAME_None, GetPlayerReach());
}

void UGrabber::GrabReleased()
{
	PhysicsHandle->ReleaseComponent();
}
