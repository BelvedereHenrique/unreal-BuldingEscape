// Copyrigth 2021

#include "OpenDoor.h"

#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "ToolContextInterfaces.h"

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
	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialAngle;
	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("$s has the open door component but bo pressureplate set!"), *GetOwner()->GetName());

	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!Pawn)
		UE_LOG(LogTemp, Error, TEXT("Unable to find player pawn"));

	ActorThatOpens = Pawn;
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) const
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, OpenAngle, DeltaTime, OpeningSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime) const
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialAngle, DeltaTime, ClosingSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}
