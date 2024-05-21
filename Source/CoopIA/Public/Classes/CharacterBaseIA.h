// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Data/Interface/Interact.h"
#include "GameFramework/Character.h"
#include "CharacterBaseIA.generated.h"

struct FGameplayTagContainer;

UCLASS()
class COOPIA_API ACharacterBaseIA : public ACharacter, public IInteract, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer ActorTags;
	
	void Hide();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void Succeeded();

	UFUNCTION(BlueprintCallable)
	void Failed(AActor* Target);
	
	UPROPERTY()
		TObjectPtr<class AAIManager> Manager;

	UPROPERTY(EditAnywhere)
		TObjectPtr<class UDA_IA> DataAssetIA;


	UPROPERTY(EditAnywhere)
		TObjectPtr<class UDA_UI> DA_UI;
		
	UPROPERTY()
		TObjectPtr<class AAIManager> Manager0;

	UPROPERTY()
		TObjectPtr<class AAIManager> Manager1;

	UPROPERTY()
		TObjectPtr<class AAIManager> CurrentManager;
};
