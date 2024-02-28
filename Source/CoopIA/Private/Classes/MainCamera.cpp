// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/MainCamera.h"

//Library
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//Components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"

//DataAsset
#include "Classes/Data/DACamera.h"

// Sets default values
AMainCamera::AMainCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Billboard);
	SpringArm->TargetArmLength = 500.0f; 	
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AMainCamera::AddPlayer(AActor* Actor)
{
	m_arrayActors.Add(Actor);
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateCamMovement();
	UpdateArmLength();
}

void AMainCamera::CalculateCamMovement()
{
	m_previousLoc = GetActorLocation();
	const FVector AverageLoc = UGameplayStatics::GetActorArrayAverageLocation(m_arrayActors);
	const FVector Lerp = UKismetMathLibrary::VLerp(m_previousLoc, AverageLoc, DACamera->m_MoveSpeed);
	SetActorLocation(Lerp);
}

void AMainCamera::UpdateArmLength()
{
	int ArrayIndex = 0;
	for (auto Actor : m_arrayActors)
	{
		m_currentPlayer = Actor;
		ArrayIndex++;
		for(int x = ArrayIndex; m_arrayActors.Num() - 1; x++)
		{
			m_arrayDistances.Add(m_currentPlayer->GetDistanceTo(m_arrayActors[x]));
		}
	}
	
	int32 IndexMaxValue = 0;
	float MaxValue = 0.f;
	UKismetMathLibrary Kismet;
	Kismet.MaxOfFloatArray(m_arrayDistances, IndexMaxValue, MaxValue);
	const float ArmLength = UKismetMathLibrary::Clamp(MaxValue, DACamera->m_MinArmLength, DACamera->m_MaxArmLength);
	SpringArm->TargetArmLength = ArmLength;
	m_arrayDistances.Empty();
}

