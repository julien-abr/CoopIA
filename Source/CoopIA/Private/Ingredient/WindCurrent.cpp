// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/WindCurrent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWindCurrent::AWindCurrent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	_cube->SetupAttachment(RootComponent);

	_cylinder = CreateDefaultSubobject<UStaticMeshComponent>("Cylinder");
	_cylinder->SetupAttachment(_cube);

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_cube);
}

// Called when the game starts or when spawned
void AWindCurrent::BeginPlay()
{
	Super::BeginPlay();

	_box->OnComponentBeginOverlap.AddDynamic(this, &AWindCurrent::OnBoxBeginOverlap);
	_box->OnComponentEndOverlap.AddDynamic(this, &AWindCurrent::OnBoxEndOverlap);
	
}

// Called every frame
void AWindCurrent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!inTheCurrent) return;

	FVector forceDirection = _cube->GetForwardVector() * windForce;

	UStaticMeshComponent* playerMesh = Cast<UStaticMeshComponent>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetRootComponent());
	if (playerMesh) playerMesh->AddForce(forceDirection);
}

void AWindCurrent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == OtherActor)
	{
		inTheCurrent = true;
	}
}

void AWindCurrent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == OtherActor)
	{
		inTheCurrent = false;
	}
}

