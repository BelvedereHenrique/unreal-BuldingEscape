// Copyrigth 2021

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	float InitialAngle;
	float DoorLastOpened = 0.f;
	float DoorCloseDelay = 2.f;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float OpeningSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float ClosingSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;

	void OpenDoor(float DeltaTime) const;
	void CloseDoor(float DeltaTime) const;
};
