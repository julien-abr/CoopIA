// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "PlayerControllerBase.generated.h"

class UStateMachineComponent;
class AMainCamera;

UCLASS()
class COOPIA_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDA_StateMachine> DA_StateMachine;
	
public:
	APlayerControllerBase();
	void Init(AMainCamera* Camera, const int Index);

	void LateInit() const;

	UStateMachineComponent* GetStateMachineComponent() const;
	const int GetPlayerIndex() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<AMainCamera> MainCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStateMachineComponent> StateMachineComponent;

	int PlayerIndex;

	bool bDoOncePosses = false;
	
};
