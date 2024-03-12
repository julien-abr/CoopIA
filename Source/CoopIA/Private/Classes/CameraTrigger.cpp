// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/CameraTrigger.h"
#include "Classes/CharacterBase.h"
#include "Classes/MainCamera.h"

//Library
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraTrigger::ACameraTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACameraTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACameraTrigger::OnBoxBeginOverlap);
	MainCamera = Cast<AMainCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCamera::StaticClass()));
}

// Called every frame
void ACameraTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacterBase* Player = Cast<ACharacterBase>(OtherActor);
    if(OtherActor && bCanOverlap)
    {
    	switch(TriggerCamera)
    	{
    		case ECameraState::FIXED:
    			MainCamera->SetFixedPosition(ActorFixedPos->GetActorLocation());
    			break;
    		case ECameraState::FOLLOW:
    			MainCamera->SetSpline(FollowSpline);
    			break;
    	}

    	bCanOverlap = false;
    }
}

