// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Interface/Interact.h"
#include "GameFramework/Character.h"
#include "CharacterBaseIA.generated.h"

class UDA_IA;
class UDA_UI;
struct FGameplayTagContainer;
class UStateMachineComponent;

UCLASS()
class COOPIA_API ACharacterBaseIA : public ACharacter, public IInteract, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
		bool bIAtoReceive;

public:
	// Sets default values for this character's properties
	ACharacterBaseIA();

	UFUNCTION(BlueprintImplementableEvent)
	void Move(FVector Destination, float AcceptanceRadius);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveToActor(AActor* Target, float AcceptanceRadius);

	void Show();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	int32 PlayerIndex;

	void Init(const int32 Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Interfaces
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override {TagContainer = ActorTags; };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Hide();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpiritNeutral();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpiritPickup(const int32 Index);
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void Succeeded();

	UFUNCTION(BlueprintCallable)
	void Failed(AActor* Target);
	
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDA_IA> DataAssetIA;


	UPROPERTY(EditAnywhere)
		TObjectPtr<UDA_UI> DA_UI;
		
	UPROPERTY()
		TObjectPtr<UStateMachineComponent> ST_Player0;

	UPROPERTY()
		TObjectPtr<UStateMachineComponent> ST_Player1;

	UPROPERTY()
		TObjectPtr<UStateMachineComponent> CurrentST;
};
