// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Interface/Interact.h"
#include "GameFramework/Character.h"
#include "CharacterBoule.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class COOPIA_API ACharacterBoule : public ACharacter, public IInteract
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContextToRemove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BouleMoveAction;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* _sphere;

public:
	// Sets default values for this character's properties
	ACharacterBoule();

	void Hide();

	void Show();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	void Move(const FInputActionValue& Value);

public:
	UPROPERTY(EditAnywhere)
	float bouleSpeed;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
