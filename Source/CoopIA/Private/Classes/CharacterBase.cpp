// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CharacterBase.h"
#include "Classes/AIManager.h"
#include "Classes/Data/EIAState.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Classes/DeathManager.h"
#include "Classes/Shield.h"
#include "Classes/GameStateBaseCoop.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/StateMachine/StateMachineComponent.h"

#include "Classes/Data/DataAsset/DAPlayer.h"
#include "Classes/Data/DataAsset/DAShield.h"
#include "Classes/Data/DataAsset/DA_UI.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacterBase);

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	if(DAPlayer)
	{
		GetCharacterMovement()->MaxWalkSpeed = DAPlayer->MaxSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}

	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACharacterBase::Init(UStateMachineComponent* StateMachineComponent)
{
	if(!ST)
	{
		ST = StateMachineComponent;
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnBoxBeginOverlap);
		SetMaterial(false);
	}
}

void ACharacterBase::ImpulseTowardActor()
{
	const UWorld* World = GetWorld();
	const AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(World));
	const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(this);
	const AActor* OtherPlayer = (Index == 0) ? GameState->GetPlayer(1) : GameState->GetPlayer(0);
	const FVector End = OtherPlayer->GetActorLocation();
	const FVector Start = GetActorLocation();
	FVector LaunchVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(World,LaunchVelocity, Start, End);
	LaunchVelocity.Z = 0;
	LaunchCharacter(LaunchVelocity, false, false);	
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupDefaultMapping();
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Neutral
		EnhancedInputComponent->BindAction(NeutralAction, ETriggerEvent::Started, this, &ACharacterBase::StartNeutral);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);

		//Shield
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Started, this, &ACharacterBase::StartShield);
		
		//Ball
		EnhancedInputComponent->BindAction(BallAction, ETriggerEvent::Started, this, &ACharacterBase::StartBall);
		
		//Spear
		EnhancedInputComponent->BindAction(SpearAction, ETriggerEvent::Started, this, &ACharacterBase::StartSpear);

		//Dead
		EnhancedInputComponent->BindAction(DeadImpulseAction, ETriggerEvent::Started, this, &ACharacterBase::ImpulseTowardActor);
	}
	else
	{
		UE_LOG(LogTemplateCharacterBase, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

int32 ACharacterBase::GetPlayerIndex_Implementation()
{
	return ST->GetPlayerIndex();
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FRotator YawRotation(0, 0, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	if (ST->GetCurrentState() == ShieldTag)
		OnMoveShield(ST->GetPlayerIndex());
	else if (ST->GetCurrentState() == NeutralTag)
		OnMoveNeutral();
}

void ACharacterBase::Hide()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ACharacterBase::Show()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void ACharacterBase::SetMaterial(bool bIsDead) const
{
	USkeletalMeshComponent* MeshPlayer = GetMesh();
	if (bIsDead)
	{
		//TODO :: implement dead mat
		MeshPlayer->SetMaterial(0, MaterialDead0);
		MeshPlayer->SetMaterial(0, MaterialDead1);
	}
	else
	{
		//Player0 == Green
		if (ST->GetPlayerIndex() == 0)
		{
			MeshPlayer->SetMaterial(0, DA_UI->PlayerGreenIA_Mat0);
			MeshPlayer->SetMaterial(1, DA_UI->PlayerGreenIA_Mat1);
			MeshPlayer->SetMaterial(2, DA_UI->PlayerGreenIA_Mat2);
			MeshPlayer->SetMaterial(3, DA_UI->PlayerGreenIA_Mat3);
		}
		//Player1 == Red
		else
		{
			MeshPlayer->SetMaterial(0, DA_UI->PlayerRedIA_Mat0);
			MeshPlayer->SetMaterial(1, DA_UI->PlayerRedIA_Mat1);
			MeshPlayer->SetMaterial(2, DA_UI->PlayerRedIA_Mat2);
			MeshPlayer->SetMaterial(3, DA_UI->PlayerRedIA_Mat3);
		}
	}
}

void ACharacterBase::SetupDefaultMapping() const
{
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(FormationMappingContext, 0);
		}
	}
}

void ACharacterBase::SetupDeadMapping() const
{
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DeadMappingContext, 0);
		}
	}
}

void ACharacterBase::StartSpear()
{
	UE_LOGFMT(LogTemp, Warning, "Input Spear - #{0}", GetPlayerIndex_Implementation());
	if(ST->CanUpdateState())
		ST->UpdateState(SpearTag);
}

void ACharacterBase::Died()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetMaterial(true);
	SetupDeadMapping();
	GetCapsuleComponent()->SetCollisionObjectType(collisionChannelDead);
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->GravityScale = 0.f;
	CharacterMovementComponent->GroundFriction = 0.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 200;
	CharacterMovementComponent->BrakingDecelerationFalling = 200;
}

void ACharacterBase::Revive() const
{
	SetMaterial(false);
	SetupDefaultMapping();
	GetCapsuleComponent()->SetCollisionObjectType(collisionChannelAlive);
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->GravityScale = 1.f;
	CharacterMovementComponent->GroundFriction = 8.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 2000;
	CharacterMovementComponent->BrakingDecelerationFalling = 1500;
}


void ACharacterBase::StartShield()
{
	UE_LOGFMT(LogTemp, Warning, "Input Shield - #{0}", GetPlayerIndex_Implementation());
	if(ST->CanUpdateState() && !bIsShieldActivate)
		ST->UpdateState(ShieldTag);
}

void ACharacterBase::StartBall()
{
	UE_LOGFMT(LogTemp, Warning, "Input Ball - #{0}", GetPlayerIndex_Implementation());
	if (!GetCharacterMovement()->IsFalling() && ST->CanUpdateState())
		ST->UpdateState(BallTag);
}

void ACharacterBase::StartNeutral()
{
	if(bIsShieldActivate && ST->CanUpdateState())
	{
		UE_LOGFMT(LogTemp, Warning, "Input Neutral - #{0}", GetPlayerIndex_Implementation());
		ST->UpdateState(NeutralTag);
	}
}

void ACharacterBase::SetupShield(class AShield* Shield)
{
	if(Shield)
	{
		ShieldActor = Shield;
	}
	
	ShieldActor->Show();
	if(DAShield)
	{
		GetCharacterMovement()->MaxWalkSpeed = DAShield->MaxSpeed;
	}
	
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ShieldMappingContext, 0);
		}
	}
	
	bIsShieldActivate = true;

	if(isBind)
		return;

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Controller->InputComponent))
	{
		isBind = true;

		// JShield Rotate Left
		EnhancedInputComponent->BindAction(ShieldRotateLeftAction, ETriggerEvent::Started, this, &ACharacterBase::ShieldRotateLeftStarted);
		EnhancedInputComponent->BindAction(ShieldRotateLeftAction, ETriggerEvent::Completed, this, &ACharacterBase::ShieldRotateLeftCompleted);
		
		// Shield Rotate Right
		EnhancedInputComponent->BindAction(ShieldRotateRightAction, ETriggerEvent::Started, this, &ACharacterBase::ShieldRotateRightStarted);
		EnhancedInputComponent->BindAction(ShieldRotateRightAction, ETriggerEvent::Completed, this, &ACharacterBase::ShieldRotateRightCompleted);
	}
}

void ACharacterBase::DeactivateShield()
{
	ShieldActor->Hide();
	bIsShieldActivate = false;
	if(DAPlayer)
	{
		GetCharacterMovement()->MaxWalkSpeed = DAPlayer->MaxSpeed;
	}
}

void ACharacterBase::ShieldRotateLeftStarted()
{
	ShieldActor->RotationLeftStarted();
}

void ACharacterBase::ShieldRotateRightStarted()
{
	ShieldActor->RotationRightStarted();
}

void ACharacterBase::ShieldRotateLeftCompleted()
{
	ShieldActor->RotationLeftCompleted();
}

void ACharacterBase::ShieldRotateRightCompleted()
{
	ShieldActor->RotationRightCompleted();
}

void ACharacterBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetCapsuleComponent()->GetCollisionObjectType() != collisionChannelDead) {return;}
		
	if(OtherActor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
	{
		const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(OtherActor);
		const int32 OwnerIndex = IPlayerInterface::Execute_GetPlayerIndex(this);
		if(Index != OwnerIndex)
		{
			//Play effect + revive
			ADeathManager* DeathManager = Cast<ADeathManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADeathManager::StaticClass()));
			DeathManager->RevivePlayer(OwnerIndex);
		}
	}
}
