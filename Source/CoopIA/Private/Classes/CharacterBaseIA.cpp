// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CharacterBaseIA.h"
#include "Classes/AIManager.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/Data/DataAsset/DA_UI.h"
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

void ACharacterBaseIA::Init(const int32 Index)
{
	USkeletalMeshComponent* MeshIA = GetMesh();
	if (Index == 0)
	{
		CurrentManager = Manager0;
		MeshIA->SetMaterial(0, DA_UI->GreenIA_Mat0);
		MeshIA->SetMaterial(1, DA_UI->GreenIA_Mat1);
	}
	else
	{
		CurrentManager = Manager1;
		MeshIA->SetMaterial(0, DA_UI->RedIA_Mat0);
		MeshIA->SetMaterial(1, DA_UI->RedIA_Mat1);	
	}
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
		if(ManagerIA->ManagerIndex == 0)
		{
			Manager0 = ManagerIA;
		}
		else
		{
			Manager1 = ManagerIA;
		}
	}

	Init(PlayerIndex);
	if(CurrentManager)
		CurrentManager->AddPlayer(this);
	
}

void ACharacterBaseIA::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	/*if(Cast<ACharacterBaseIA>(OtherActor))
	{
		const FVector impulseForce = Hit.ImpactPoint * DataAssetIA->CollisionImpulseMultiplier;
		//GetCharacterMovement()->AddImpulse(impulseForce, true);
	}
	else if(Cast<ACharacterBase>(OtherActor))
	{
		const FVector impulseForce = Hit.ImpactPoint * -1.f;
		//GetCharacterMovement()->AddImpulse(impulseForce, true);
	}*/
	
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

void ACharacterBaseIA::Hide()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ACharacterBaseIA::Show()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void ACharacterBaseIA::Succeeded()
{
	if(CurrentManager)
		CurrentManager->IASucceededTransition();
	Hide();
}

void ACharacterBaseIA::Failed(AActor* Target)
{
	MoveToActor(Target, DataAssetIA->RetryAcceptanceRadius);
}