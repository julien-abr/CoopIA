// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Spear.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Classes/Data/DataAsset/DASpear.h"
#include "Classes/AIManager.h"
#include "Classes/Data/EIAState.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpear::ASpear()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	BoxCollision->SetupAttachment(RootComponent);
	
	SpearMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpearMesh");
	SpearMesh->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ASpear::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetSimulatePhysics(false);
}

void ASpear::SetAIManager(class AAIManager* Manager)
{
	AIManager = Manager;
}

void ASpear::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(FormationMappingContext, 0);
			Subsystem->AddMappingContext(SpearMappingContext, 0);
		}
	}
}

void ASpear::Tick(float DeltaSeconds)
{
	//TODO::Update timer with FTimerHandle remaining time instead of tick
	Super::Tick(DeltaSeconds);

	if(bCanUpdateTimer)
	{
		HoldTimer += DeltaSeconds;
	}

	float Velocity = GetCharacterMovement()->Velocity.Length();

	if(Velocity > 50 && SpearState != ESpearState::DASHING)
	{
		SpearState = ESpearState::DASHING;
	}
	
	if(Velocity < 50  && SpearState != ESpearState::STATIC)
	{
		SpearState = ESpearState::STATIC;
	}
}

void ASpear::StartHold()
{
	if(!bCanDash) {return;}

	GetCharacterMovement()->MaxWalkSpeed = DASpear->SpearDashMoveSpeed;
	HoldTimer = 0.f;
	bCanUpdateTimer = true;
	bStartHold = true;
}

void ASpear::DashUp()
{
	if(!bCanDash || !bStartHold) {return;}
	
	bCanUpdateTimer = false;

	const FVector LaunchZForce = FVector(0.f, 0.f, DASpear->DashZBaseVelocity);
	LaunchCharacter(LaunchZForce, false, false);

	GetWorld()->GetTimerManager().SetTimer(TimerDashForward, this, &ASpear::DashForward, DASpear->TimerBetweenDashUPForward, false);
	bCanDash = false;
	bStartHold = false;
	GetWorld()->GetTimerManager().SetTimer(TimerDash, FTimerDelegate::CreateLambda([&] { bCanDash = true; }), DASpear->TimerDash, false);
}

void ASpear::DashForward()
{
	if(HoldTimer > DASpear->SpearHoldTimer)
	{
		HoldTimer =  DASpear->SpearHoldTimer;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("HoldTimer : %f"), HoldTimer);
	const float DashPower = UKismetMathLibrary::Lerp(DASpear->SpearMinPower, DASpear->SpearMaxPower, HoldTimer/DASpear->SpearHoldTimer);
	const FVector DashForce = UKismetMathLibrary::GetForwardVector(GetActorRotation()) * DashPower;

	//UE_LOG(LogTemp, Warning, TEXT("DashPower : %f"), DashPower);
	LaunchCharacter(DashForce, false, false);
	
	GetCharacterMovement()->MaxWalkSpeed = DASpear->SpearMoveSpeed;
}

// Called to bind functionality to input
void ASpear::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASpear::StartHold);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ASpear::DashUp);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpear::Look);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpear::Move);
		
		//Ball
		EnhancedInputComponent->BindAction(BallAction, ETriggerEvent::Started, this, &ASpear::StartBall);

		//Neutral
		EnhancedInputComponent->BindAction(NeutralAction, ETriggerEvent::Started, this, &ASpear::StartNeutral);

		//Shield
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Started, this, &ASpear::StartShield);
	}
}

void ASpear::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASpear::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));
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

void ASpear::StartBall()
{
	UE_LOG(LogTemp, Warning, TEXT("Input Ball"));
	AIManager->UpdateState(EIAState::BALL);
}

void ASpear::StartNeutral()
{
	UE_LOG(LogTemp, Warning, TEXT("Input Neutral"));
	AIManager->UpdateState(EIAState::RANDOM_MOVE);
}

void ASpear::StartShield()
{
	UE_LOG(LogTemp, Warning, TEXT("Input Shield"));
	AIManager->UpdateState(EIAState::SHIELD);
}

void ASpear::Hide()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

void ASpear::Show()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}
