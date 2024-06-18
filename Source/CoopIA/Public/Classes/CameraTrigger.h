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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isTwoPlayer = true;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWallActivate();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	AMainCamera* MainCamera;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"))
	ECameraState TriggerCamera;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"), BlueprintReadOnly)
	TObjectPtr<AActor> WallActor;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition="TriggerCamera == ECameraState::FIXED"))
	AActor* ActorFixedPos;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition="TriggerCamera == ECameraState::FOLLOW"))
	ASpline* FollowSpline;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true"))
	EZoneType ZoneType;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition="ZoneType == EZoneType::Puzzle"))
	ELevelSide LevelSide;

	UPROPERTY(EditAnywhere, Category = "TriggerOptions", meta = (AllowPrivateAccess = "true", EditCondition = "ZoneType == EZoneType::Puzzle"))
	AActor* ActorRespawn;

	UPROPERTY()
	TObjectPtr<AActor> Player0;

	UPROPERTY()
	TObjectPtr<AActor> Player1;
	
	bool bCanOverlap = true;

};