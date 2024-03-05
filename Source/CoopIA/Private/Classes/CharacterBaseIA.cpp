// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CharacterBaseIA.h"
#include "Classes/AIControllerBase.h"
#include "Classes/AIManager.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/CharacterBase.h"
#include "Components/CapsuleComponent.h"

//lib
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterBaseIA::ACharacterBaseIA()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACharacterBaseIA::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacterBaseIA::OnHit);

	TArray<AActor*> arrayManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIManager::StaticClass(), arrayManagers);

	for (auto ManagerActor : arrayManagers)
	{
		AAIManager* ManagerIA = Cast<AAIManager>(ManagerActor);
		if(ManagerIA->ManagerIndex == PlayerIndex)
		{
			Manager = ManagerIA;
		}
	}
	Manager->AddPlayer(this);
}

void ACharacterBaseIA::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(Cast<ACharacterBaseIA>(OtherActor))
	{
		const FVector impulseForce = Hit.ImpactPoint * DataAssetIA->CollisionImpulseMultiplier;
		GetCharacterMovement()->AddImpulse(impulseForce, true);
	}
	else if(Cast<ACharacterBase>(OtherActor))
	{
		const FVector impulseForce = Hit.ImpactPoint * -1.f;
		GetCharacterMovement()->AddImpulse(impulseForce, true);
	}
}


void ACharacterBaseIA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBaseIA::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBaseIA::Move(FVector Destination, bool bIsTransition, float AcceptanceRadius)
{
	AAIControllerBase* ControllerIA = Cast<AAIControllerBase>(GetController());
	EPathFollowingRequestResult::Type MoveResult = ControllerIA->MoveToLocation(Destination, AcceptanceRadius, false);

	//Need to restart move only if this is a transition to transfo, dont need if random move
	if(!bIsTransition) return;

	switch(MoveResult)
	{
		case EPathFollowingRequestResult::Type::Failed:
			Move(Destination, true, DataAssetIA->RetryAcceptanceRadius);
			break;
		case EPathFollowingRequestResult::Type::RequestSuccessful:
			Manager->IASucceededTransition();
			break;
	}
}