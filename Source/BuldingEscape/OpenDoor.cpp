// Copyrigth 2021

#include "OpenDoor.h"

#include "Engine.h"

UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += InitialAngle;
	FindAudioComponent();
	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("$s has the open door component but bo pressureplate set!"), *GetOwner()->GetName());
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetPressurePlateMass() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, OpenAngle, DeltaTime, OpeningSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
	CloseDoorSoundPlayed = false;
	if (AudioComponent && !OpenDoorSoundPlayed)
	{
		AudioComponent->Play();
		OpenDoorSoundPlayed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialAngle, DeltaTime, ClosingSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
	OpenDoorSoundPlayed = false;
	if (AudioComponent && !CloseDoorSoundPlayed)
	{
		AudioComponent->Play();
		CloseDoorSoundPlayed = true;
	}
}

float UOpenDoor::GetPressurePlateMass() const
{
	float TotalMass = .0f;

	TArray<AActor*> Actors;
	PressurePlate->GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
}
