// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/MainCamera.h"

//Library
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//Components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BillboardComponent.h"
#include "Classes/Spline.h"

//DataAsset
#include "Classes/Data/DACamera.h"
#include "Components/SplineComponent.h"

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

void AMainCamera::SetPlayer(AActor* Actor, int16 Index)
{
	if(Actor)
	{
		TObjectPtr<class AActor>& ActorPlayer = (Index == 0) ? m_ActorPlayer0 : m_ActorPlayer1;
		ActorPlayer = Actor;
	}
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

	Spline = Cast<ASpline>(UGameplayStatics::GetActorOfClass(GetWorld(), DefaultSpline));
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CameraState == ECameraState::FOLLOW)
	{
		FollowPlayers();
	}
}
void AMainCamera::FollowPlayers()
{
	TArray<AActor*> m_arrayActors;

	if(m_ActorPlayer0)
		m_arrayActors.Add(m_ActorPlayer0);
	if (m_ActorPlayer1)
		m_arrayActors.Add(m_ActorPlayer1);

	SpringArm->TargetArmLength = DACamera->DistanceCamFollow;
	const FVector AverragePlayersLoc = UGameplayStatics::GetActorArrayAverageLocation(m_arrayActors);
	const FVector Current = SpringArm->GetComponentLocation();
	const FVector Target = Spline->SplineComponent->FindLocationClosestToWorldLocation(AverragePlayersLoc, ESplineCoordinateSpace::World) - FVector(DACamera->Offset, 0.f, 0.f);
	const FVector SpringArmPos = UKismetMathLibrary::VInterpTo(Current, Target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),DACamera->InterpSpeed);
	
	SetActorLocation(SpringArmPos);
	SetActorRotation(DACamera->FollowCamRotation);

}

void AMainCamera::SetFixedPosition(FTransform Transform)
{
	CameraState = ECameraState::FIXED;

	SpringArm->TargetArmLength = 0.f;
	SetActorTransform(Transform);
}

void AMainCamera::SetSpline(class ASpline* NewSpline)
{
	Spline = NewSpline;
	CameraState = ECameraState::FOLLOW;
}