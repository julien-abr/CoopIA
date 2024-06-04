// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Canon.h"

#include "Classes/Ball.h"
#include "Data/Interface/Interact.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_cube = CreateDefaultSubobject<UStaticMeshComponent>("Canon");
	_cube->SetupAttachment(RootComponent);

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_cube);

	_arrow = CreateDefaultSubobject<UArrowComponent>("LaunchDirection");
	_arrow->SetupAttachment(_cube);

}

// Called when the game starts or when spawned
void ACanon::BeginPlay()
{
	Super::BeginPlay();

	_box->OnComponentBeginOverlap.AddDynamic(this, &ACanon::OnBoxBeginOverlap);
	_box->OnComponentEndOverlap.AddDynamic(this, &ACanon::OnBoxEndOverlap);
	 
}

// Called every frame
void ACanon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACanon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()) && Cast<ABall>(OtherActor))
	{
		OtherActor->SetActorLocation(_arrow->GetComponentLocation());
		InteractInterface = Cast<IInteract>(OtherActor);
		InteractInterface->Execute_LockPropulseBoule(OtherActor);
	}
}

void ACanon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{

}

