// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingredient/Canon.h"

#include "Classes/Ball.h"
#include "Data/Interface/Interact.h"
#include "Data/DABall.h"

// Sets default values
ACanon::ACanon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_cube = CreateDefaultSubobject<UStaticMeshComponent>("Canon");
	_cube->SetupAttachment(RootComponent);

	_box = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	_box->SetupAttachment(_cube);

	_arrowL = CreateDefaultSubobject<UArrowComponent>("LaunchDirectionLeft");
	_arrowL->SetupAttachment(_cube);

	_arrowR = CreateDefaultSubobject<UArrowComponent>("LaunchDirectionRight");
	_arrowR->SetupAttachment(_cube);

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

void ACanon::TriggerLaunch()
{
	//IInteract::Execute_LaunchPropulseBoule(BouleToLaunchP1, _arrowL->GetForwardVector(), launchForce, maxVelocityBoule);
	InteractInterfaceP1->Execute_LockPropulseBoule(BouleToLaunchP1, false);
	InteractInterfaceP1->Execute_LaunchPropulseBoule(BouleToLaunchP1, _arrowL->GetForwardVector(), launchForce, maxVelocityBoule);
	InteractInterfaceP2->Execute_LockPropulseBoule(BouleToLaunchP2, false);
	InteractInterfaceP2->Execute_LaunchPropulseBoule(BouleToLaunchP2, _arrowR->GetForwardVector(), launchForce, maxVelocityBoule);
	
}

void ACanon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()) && Cast<ABall>(OtherActor))
	{
		//InteractInterface = Cast<IInteract>(OtherActor);
		//InteractInterfaces.Emplace(Cast<IInteract>(OtherActor));

		if (InteractInterfaceP1 == nullptr) InteractInterfaceP1.SetInterface(Cast<IInteract>(OtherActor));
		else InteractInterfaceP2.SetInterface(Cast<IInteract>(OtherActor));
		Cast<IInteract>(OtherActor)->Execute_LockPropulseBoule(OtherActor, true);

		if (BouleToLaunchP1 == nullptr)
		{
			BouleToLaunchP1 = OtherActor;
			OtherActor->SetActorLocation(_arrowL->GetComponentLocation());
			//GetWorld()->GetTimerManager().SetTimer<ACanon>(TriggerLaunchTimerHandle, this, &ACanon::TriggerLaunch, DABall->loadTimeCanon, false);
		}
		else
		{
			BouleToLaunchP2 = OtherActor;
			OtherActor->SetActorLocation(_arrowR->GetComponentLocation());
			GetWorld()->GetTimerManager().SetTimer<ACanon>(TriggerLaunchTimerHandle, this, &ACanon::TriggerLaunch, DABall->loadTimeCanon, false);
		}

		/*BoulesToLaunch.Emplace(OtherActor);
		if (BoulesToLaunch.Num() == 1) OtherActor->SetActorLocation(_arrowL->GetComponentLocation());
		else OtherActor->SetActorLocation(_arrowR->GetComponentLocation());*/

		/*if (BoulesToLaunch.Num() == 1)
			GetWorld()->GetTimerManager().SetTimer<ACanon>(TriggerLaunchTimerHandle, this, &ACanon::TriggerLaunch, DABall->loadTimeCanon, false);*/
	}
}

void ACanon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()) && Cast<ABall>(OtherActor))
	{
		/*BoulesToLaunch.RemoveSingle(OtherActor);
		InteractInterfaces.RemoveSingle(Cast<IInteract>(OtherActor));*/
		if (OtherActor == BouleToLaunchP1) BouleToLaunchP1 = nullptr;
		else if (OtherActor == BouleToLaunchP2) BouleToLaunchP2 = nullptr;

		if (OtherActor == BouleToLaunchP1) InteractInterfaceP1->Execute_LockPropulseBoule(BouleToLaunchP1, false);
		else if (OtherActor == BouleToLaunchP2) InteractInterfaceP2->Execute_LockPropulseBoule(BouleToLaunchP2, false);
	}

	/*if (InteractInterface && BouleToLaunch)
		InteractInterface->Execute_LockPropulseBoule(BouleToLaunch, false);*/
}

