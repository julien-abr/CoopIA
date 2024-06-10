// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/StateMachine/StateMachineComponent.h"

#include "NavigationSystem.h"
#include "Classes/Ball.h"
#include "Classes/CharacterBase.h"
#include "Classes/CharacterBaseIA.h"
#include "Classes/PlayerControllerBase.h"
#include "Classes/Spear.h"
#include "Classes/Data/DataAsset/DA_IA.h"
#include "Classes/Data/DataAsset/DA_StateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tool/HexBehaviour.h"

class AHexBehaviour;

UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::UpdateState(const FGameplayTag& Tag)
{
	if(!DA_StateMachine){return;}

	NextTag = Tag;
	InitializeState(DA_StateMachine->TransitionState);
}

void UStateMachineComponent::UpdateStateFromTransition(const FGameplayTag& Tag)
{
	if(!DA_StateMachine){return;}
	
	InitializeState(Tag);
	NextTag = FGameplayTag::EmptyTag;
}

void UStateMachineComponent::Init(APlayerControllerBase* ControllerRef, UDA_StateMachine* DA_StateMachineRef)
{
	if(!DA_StateMachineRef || !ControllerRef){return;}

	PlayerController = ControllerRef;
	DA_StateMachine = DA_StateMachineRef;
	PlayerIndex = PlayerController->GetPlayerIndex();
	InitializeState(DA_StateMachine->DefaultState);
	
	GetWorld()->GetTimerManager().SetTimer(HandleHexRaycast, this, &UStateMachineComponent::FindLastHex, 1, true);
}

void UStateMachineComponent::GetOtherST()
{
	TArray<AActor*> PlayerControllerBaseFounded;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerControllerBase::StaticClass(), PlayerControllerBaseFounded);

	for (auto Controller : PlayerControllerBaseFounded)
	{
		APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(Controller);
		if(PlayerControllerBase != PlayerController)
		{
			OtherST = PlayerControllerBase->GetStateMachineComponent();
		}
	}
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentState)
		CurrentState->OnStateTick();
}

void UStateMachineComponent::InitializeState(const FGameplayTag& Tag)
{
	 if(CurrentState)
	 	CurrentState->OnStateLeave();
	
	 if(const TSubclassOf<UState>* State = DA_StateMachine->StatesMap.Find(Tag))
	 {
	 	CurrentState = NewObject<UState>(this, *State);
	 	UStateMachineComponent* ST = this;
	 	CurrentStateTag = Tag;
	 	StateHistoric.Add(Tag);
	 	CurrentState->OnStateEnter(ST);
	 }
}

void UStateMachineComponent::AddToHistoric(const FGameplayTag& Tag)
{
	StateHistoric.Add(Tag);
}

void UStateMachineComponent::AddPlayer(ACharacterBaseIA* IACharacter)
{
	ArrayIA.Add(IACharacter);
}

const FGameplayTag& UStateMachineComponent::GetCurrentState() const
{
	return CurrentStateTag;
}

const int UStateMachineComponent::GetPlayerIndex() const
{
	return PlayerIndex;
}

const AActor* UStateMachineComponent::GetCurrentActor() const
{
	return CurrentActor;
}

const int UStateMachineComponent::GetIACount() const
{
	return ArrayIA.Num();
}

void UStateMachineComponent::RemoveAI(ACharacterBaseIA* CharacterAI)
{
	ArrayIA.Remove(CharacterAI);
}

TArray<ACharacterBaseIA*> UStateMachineComponent::SplitAI()
{
	TArray<ACharacterBaseIA*> CharacterIASplited;
	
	int numberIASplited = 0;
	while (numberIASplited < ArrayIA.Num() / 2)
	{
		CharacterIASplited.Add(ArrayIA[numberIASplited]);
		numberIASplited++;
	}

	for (auto IA_Splited : CharacterIASplited)
	{
		ArrayIA.Remove(IA_Splited);
	}

	return CharacterIASplited;
}

void UStateMachineComponent::HidePrevious() const
{
	const FGameplayTag& PreviousTag = GetLastTagTransitionExcluded();
	if(PreviousTag == DA_StateMachine->NeutralState)
	{
		if(Player)
			Player->Hide();
	}
	if(PreviousTag == DA_StateMachine->BallState)
	{
		//BALL
		if(Player)
			Player->Hide();
		if(BallActor)
			BallActor->Hide();
	}
	else if(PreviousTag == DA_StateMachine->SpearState)
	{
		//SPEAR
		if(Player)
			Player->Hide();
		if(SpearActor)
			SpearActor->Hide();
	}
	else if(PreviousTag == DA_StateMachine->ShieldState)
	{
		//SHIELD
		if(Player)
		{
			Player->Hide();
			Player->DeactivateShield();
		}
	}
	else if(PreviousTag == DA_StateMachine->DeadState)
	{
		//DEAD
		if(Player)
			Player->Hide();
	}
}

const FGameplayTag& UStateMachineComponent::GetLastTagTransitionExcluded() const
{
	if(!StateHistoric.Num() - 2 >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EXIT IN GET LAST TAG"));
		return FGameplayTag::EmptyTag;
	}
	
	for (int x = StateHistoric.Num() - 2; x >= 0; x--)
	{
		//UE_LOG(LogTemp, Warning, TEXT("x : %d : %s"),x, *StateHistoric[x].ToString());
		if(StateHistoric[x] != DA_StateMachine->TransitionState)
		{
			UE_LOG(LogTemp, Warning, TEXT("Return %s"),*StateHistoric[x].ToString());
			return StateHistoric[x];
		}
	}
	return FGameplayTag::EmptyTag;
}

const FVector UStateMachineComponent::GetPositionForState() const
{
	const FGameplayTag& PreviousTag = GetLastTagTransitionExcluded();
	UE_LOG(LogTemp,Warning,TEXT("Previous Tag : %s"), *PreviousTag.ToString());
	
	if(PreviousTag == DA_StateMachine->InitState)
	{
		//NEUTRAl
		return Player->GetActorLocation();
	}
	if(PreviousTag == DA_StateMachine->NeutralState)
	{
		//NEUTRAl
		return Player->GetActorLocation();
	}
	if(PreviousTag == DA_StateMachine->SpearState)
	{
		//SPEAR
		return SpearActor->GetActorLocation();
	}
	if(PreviousTag == DA_StateMachine->ShieldState)
	{
		//SHIELD
		return Player->GetActorLocation();
	}
	return FVector();
}

void UStateMachineComponent::FindLastHex()
{
	if(Player && CurrentActor)
	{
		FHitResult HitResult;
		FVector Start = CurrentActor->GetActorLocation();
		FVector End = Start - (FVector::UpVector * 200);	
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(CurrentActor);
		
		if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params, FCollisionResponseParams()))
		{
			AHexBehaviour* Hex = Cast<AHexBehaviour>(HitResult.GetActor());
			if(Hex)
			{
				PlayerLastHexPos = Hex->GetRespawnLoc();
			}
		}
	}
}

void UStateMachineComponent::IARandomMove()
{
	for(auto IA : ArrayIA)
	{
		if(CurrentActor)
		{
			const FVector halfSize = FVector(DA_StateMachine->DataAssetIA->RandomMoveDistanceFromPlayer, DA_StateMachine->DataAssetIA->RandomMoveDistanceFromPlayer,CurrentActor->GetActorLocation().Z);
			const FVector Destination = UKismetMathLibrary::RandomPointInBoundingBox(CurrentActor->GetActorLocation(), halfSize);
			IA->Move(Destination, DA_StateMachine->DataAssetIA->BaseAcceptanceRadius);
		}
	}
}

void UStateMachineComponent::ShowAndTeleportIA()
{
	const FVector PlayerLoc = CurrentActor->GetActorLocation();
	const float DestinationZ = PlayerLoc.Z;

	for(auto IA : ArrayIA)
	{
		FNavLocation NavLoc;
		FRotator Rotation = IA->GetActorRotation();
		bool bFindDestination = NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 300.f, NavLoc);
		if(bFindDestination)
		{
			FVector TargetLoc = NavLoc.Location;
			IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
			IA->Show();
		}
		else
		{
			ShowAndTeleportIAFailed(IA, PlayerLoc, DestinationZ);
		}
	}
}

void UStateMachineComponent::ShowAndTeleportIAFailed(ACharacterBaseIA* IA, FVector PlayerLoc, float DestinationZ)
{
	FNavLocation NavLoc;
	FRotator Rotation = IA->GetActorRotation();
	bool bFindDestination = NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 200.f, NavLoc);
	if(bFindDestination)
	{
		FVector TargetLoc = NavLoc.Location;
		IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
		//IA->TeleportTo(FVector(TargetLoc.X, TargetLoc.Y, TargetLoc.Z + DestinationZ), Rotation);
		IA->Show();
	}
	else
	{
		ShowAndTeleportIAFailed(IA, PlayerLoc, DestinationZ);
	}
}

void UStateMachineComponent::HideIA()
{
	for(auto IA : ArrayIA)
	{
		IA->Hide();
	}
}
void UStateMachineComponent::DestroyIA()
{
	for(auto IA : ArrayIA)
	{
		IA->Destroy();
	}
	ArrayIA.Empty();
}



