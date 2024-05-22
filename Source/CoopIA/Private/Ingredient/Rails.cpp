// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Rails.h"

#include "Classes/Ball.h"
#include "Classes/CharacterBoule.h"
#include "Data/Interface/Interact.h"

// Sets default values
ARails::ARails()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_root = CreateDefaultSubobject<USceneComponent>("Root");
	_root->SetupAttachment(RootComponent);

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_root);

	_arrow = CreateDefaultSubobject<UArrowComponent>("RailDirection");
	_arrow->SetupAttachment(_root);

	_top = CreateDefaultSubobject<UStaticMeshComponent>("Top");
	_top->SetupAttachment(_root);
	_bottom = CreateDefaultSubobject<UStaticMeshComponent>("Bottom");
	_bottom->SetupAttachment(_root);
	_left = CreateDefaultSubobject<UStaticMeshComponent>("Left");
	_left->SetupAttachment(_root);
	_right = CreateDefaultSubobject<UStaticMeshComponent>("Right");
	_right->SetupAttachment(_root);
}

// Called when the game starts or when spawned
void ARails::BeginPlay()
{
	Super::BeginPlay();

	_box->OnComponentBeginOverlap.AddDynamic(this, &ARails::OnBoxBeginOverlap);
	_box->OnComponentEndOverlap.AddDynamic(this, &ARails::OnBoxEndOverlap);

}

// Called every frame
void ARails::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARails::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass())&& Cast<ABall>(OtherActor))
	{
		InteractInterface = Cast<IInteract>(OtherActor);
		InteractInterface->Execute_ActivateRail(OtherActor, _arrow->GetForwardVector());
		OtherActor->SetActorLocation(_arrow->GetComponentLocation());

		_top->AddRelativeLocation(FVector(0, 0, - 100));
		_bottom->AddRelativeLocation(FVector(0, 0, 100));
		_left->AddRelativeLocation(FVector(0, -100, 0));
		_right->AddRelativeLocation(FVector(0, 100, 0));
	}
}

void ARails::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()) && Cast<ABall>(OtherActor))
	{
		InteractInterface = Cast<IInteract>(OtherActor);
		InteractInterface->Execute_DeactivateRail(OtherActor);

		_top->AddRelativeLocation(FVector(0, 0, 100));
		_bottom->AddRelativeLocation(FVector(0, 0, -100));
		_left->AddRelativeLocation(FVector(0, 100, 0));
		_right->AddRelativeLocation(FVector(0, -100, 0));
	}
}

