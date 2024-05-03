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
#include "Classes/Shield.h"
#include "Classes/GameStateBaseCoop.h"

#include "Classes/Data/DataAsset/DAPlayer.h"
#include "Classes/Data/DataAsset/DAShield.h"
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

void ACharacterBase::Init(AAIManager* Manager)
{
	AIManager = Manager;
	GetMesh()->GetMaterial(0);
	GetMesh()->GetMaterial(1);
}

void ACharacterBase::ImpulseTowardActor()
{
	const UWorld* World = GetWorld();
	AGameStateBaseCoop* GameState = Cast<AGameStateBaseCoop>(UGameplayStatics::GetGameState(World));
	const int32 Index = IPlayerInterface::Execute_GetPlayerIndex(this);
	const AActor* OtherPlayer = (Index == 0) ? GameState->GetPlayer(1) : GameState->GetPlayer(0);
	const FVector End = OtherPlayer->GetActorLocation();
	GetCapsuleComponent()->SetCollisionObjectType(collisionChannelDead);
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->GravityScale = 0.f;
	CharacterMovementComponent->GroundFriction = 0.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 200;
	CharacterMovementComponent->BrakingDecelerationFalling = 200;
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

EIAState ACharacterBase::GetAIState_Implementation()
{
	if(bIsShieldActivate)
	{
		return EIAState::RANDOM_MOVE;
	}
	
	return EIAState::SHIELD;
}

int32 ACharacterBase::GetPlayerIndex_Implementation()
{
	return AIManager->ManagerIndex;
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
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
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
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

void ACharacterBase::SetupDefaultMapping()
{
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(FormationMappingContext, 0);
		}
	}
}

void ACharacterBase::SetupDeadMapping()
{
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
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
	UE_LOG(LogTemp, Warning, TEXT("Input Spear"));
	AIManager->UpdateState(EIAState::SPEAR);
}

void ACharacterBase::Died()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	GetMesh()->SetMaterial(0, MaterialDead0);
	GetMesh()->SetMaterial(1, MaterialDead1);
	SetupDeadMapping();
}

void ACharacterBase::Revive()
{
	GetMesh()->SetMaterial(0, MaterialAlive0);
	GetMesh()->SetMaterial(1, MaterialAlive1);
	SetupDefaultMapping();
}


void ACharacterBase::StartShield()
{
	UE_LOG(LogTemp, Warning, TEXT("Input Shield"));
	AIManager->UpdateState(EIAState::SHIELD);
	if(DAShield)
	{
		GetCharacterMovement()->MaxWalkSpeed = DAShield->MaxSpeed;
	}
}

void ACharacterBase::StartBall()
{
	UE_LOG(LogTemp, Warning, TEXT("Input Ball"));
	AIManager->UpdateState(EIAState::BALL);
}

void ACharacterBase::StartNeutral()
{
	if(bIsShieldActivate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Neutral"));
		AIManager->UpdateState(EIAState::RANDOM_MOVE);
	}
}

void ACharacterBase::SetupShield(class AShield* Shield)
{
	if(Shield)
	{
		ShieldActor = Shield;
	}
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
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
