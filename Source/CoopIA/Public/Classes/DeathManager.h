// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DeathManager.generated.h"

class AAIManager;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<AAIManager> AIManager0;

	UPROPERTY()
	TObjectPtr<AAIManager> AIManager1;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	FGameplayTag PlayerTag;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	FGameplayTag AITag;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RevivePlayer();
};
