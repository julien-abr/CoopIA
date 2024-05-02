// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCamera.h"
#include "Components/BoxComponent.h"
#include "Classes/Data/Enum/ZoneType.h"
#include "Data/Enum/ELevelSide.h"
#include "GameFramework/Actor.h"
#include "CameraTrigger.generated.h"

UCLASS()
class COOPIA_API ACameraTrigger : public AActor
{
	GENERATED_BODY()
	
	/** SpringArm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
public:	
	// Sets default values for this actor's properties
	ACameraTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
         
	UPROPERTY()
	AMainCamera* MainCamera;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"))
	ECameraState TriggerCamera;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition="TriggerCamera == ECameraState::FIXED"))
	AActor* ActorFixedPos;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition="TriggerCamera == ECameraState::FOLLOW"))
	ASpline* FollowSpline;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"))
	EZoneType ZoneType;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"))
	ELevelSide LevelSide;

	bool bCanOverlap = true;
};
