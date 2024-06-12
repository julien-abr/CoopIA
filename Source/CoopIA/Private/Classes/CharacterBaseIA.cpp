// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CharacterBaseIA.h"
#include "Classes/AIManager.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/Data/DataAsset/DA_UI.h"
#include "Classes/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Classes/StateMachine/StateMachineComponent.h"

//lib
#include "Classes/PlayerControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterBaseIA::ACharacterBaseIA()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

void ACharacterBaseIA::Init(const int32 Index)
{
	USkeletalMeshComponent* MeshIA = GetMesh();
	if (Index == 0)
	{
		CurrentST = ST_Player0;
		MeshIA->SetMaterial(0, DA_UI->GreenIA_Mat0);
		MeshIA->SetMaterial(1, DA_UI->GreenIA_Mat1);
	}
	else
	{
		CurrentST = ST_Player1;
		MeshIA->SetMaterial(0, DA_UI->RedIA_Mat0);
		MeshIA->SetMaterial(1, DA_UI->RedIA_Mat1);	
	}
}

// Called when the game starts or when spawned
void ACharacterBaseIA::BeginPlay()
{
	Super::BeginPlay();

	if(bIAtoReceive)
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBaseIA::OnBoxBeginOverlap);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacterBaseIA::OnHit);

	TArray<AActor*> ArrayPlayerControllerBase;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerControllerBase::StaticClass(), ArrayPlayerControllerBase);

	for (const auto ActorController : ArrayPlayerControllerBase)
	{
		const APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(ActorController);
		if(PlayerControllerBase->GetPlayerIndex() == 0)
		{
			ST_Player0 = PlayerControllerBase->GetStateMachineComponent();
		}
		else
		{
			ST_Player1 =  PlayerControllerBase->GetStateMachineComponent();
		}
	}

	if (!bIAtoReceive)
	{
		Init(PlayerIndex);
		if (CurrentST)
			CurrentST->AddPlayer(this);
	}
	else
	{
		OnSpiritNeutral();
	}
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
	//if(CurrentST)
		//CurrentST->IASucceededTransition();
	Hide();
}

void ACharacterBaseIA::Failed(AActor* Target)
{
	MoveToActor(Target, DataAssetIA->RetryAcceptanceRadius);
}

void ACharacterBaseIA::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIAtoReceive) { return; }

	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UPlayerInterface::StaticClass()))
	{
		ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
		if (!Character || Character->HasShieldActivate()) { return; }

		const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(OtherActor);
		Init(Index);
		if (CurrentST)
			CurrentST->AddPlayer(this);
		bIAtoReceive = false;

		OnSpiritPickup();
	}
}
