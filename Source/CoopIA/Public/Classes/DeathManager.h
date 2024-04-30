// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/Data/Enum/PlayerGlobalState.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DeathManager.generated.h"

class AAIManager;
class UBoxComponent;

// Delegate signature
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerGlobalStateChangedSignature, int32 /* PlayerIndex */, EPlayerGlobalState /* NewPlayerState */);

UCLASS()
class COOPIA_API ADeathManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DeathZone;
	
public:
	// Sets default values for this actor's properties
	ADeathManager();

	void Init(TArray<AAIManager*>& ArrayAIManager);
	
	FOnPlayerGlobalStateChangedSignature OnPlayerGlobalStateChangedDelegate;
	
	void RevivePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:


	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	FGameplayTag PlayerTag;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	FGameplayTag AITag;

	UPROPERTY()
	TObjectPtr<AActor> PlayerToRevive;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
