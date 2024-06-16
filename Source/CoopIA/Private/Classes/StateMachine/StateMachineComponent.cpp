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

	for (const auto Controller : PlayerControllerBaseFounded)
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
	if(IACharacter)
		ArrayIA.Add(IACharacter);
}

const FGameplayTag& UStateMachineComponent::GetCurrentState() const
{
	return CurrentStateTag;
}

int UStateMachineComponent::GetPlayerIndex() const
{
	return PlayerIndex;
}

const AActor* UStateMachineComponent::GetCurrentActor() const
{
	return CurrentActor;
}

int UStateMachineComponent::GetIACount() const
{
	return ArrayIA.Num();
}

void UStateMachineComponent::RemoveAI(ACharacterBaseIA* CharacterAI)
{
	if(CharacterAI)
	{
		if(ArrayIA.Contains(CharacterAI))
			ArrayIA.Remove(CharacterAI);
	}
}

TArray<ACharacterBaseIA*> UStateMachineComponent::SplitAI()
{
	TArray<ACharacterBaseIA*> CharacterIASplited;

	if(ArrayIA.Num() == 1)
		return CharacterIASplited;

	int numberIASplited = 0;
	while (numberIASplited < ArrayIA.Num() / 2)
	{
		CharacterIASplited.Add(ArrayIA[numberIASplited]);
		numberIASplited++;
	}

	for (auto IA_Splited : CharacterIASplited)
	{
		if(!IA_Splited) {continue;}

		IA_Splited->Init(OtherST->PlayerIndex);
		ArrayIA.Remove(IA_Splited);
	}

	return CharacterIASplited;
}

bool UStateMachineComponent::CanUpdateState() const
{
	return CurrentStateTag != DA_StateMachine->TransitionState;
}

void UStateMachineComponent::Hide(const FGameplayTag& Tag) const
{
	if(Tag == DA_StateMachine->NeutralState)
	{
		if(Player)
			Player->Hide();
	}
	else if(Tag == DA_StateMachine->BallState)
	{
		//BALL
		if(BallActor)
			BallActor->Hide();
	}
	else if(Tag == DA_StateMachine->SpearState)
	{
		//SPEAR
		if(SpearActor)
			SpearActor->Hide();
	}
	else if(Tag == DA_StateMachine->ShieldState)
	{
		//SHIELD
		if(Player)
			Player->Hide();
	}
	else if(Tag == DA_StateMachine->DeadState)
	{
		//DEAD
		if(Player)
			Player->Hide();
	}
	else if(Tag == DA_StateMachine->ReviveState)
    {
    	//REVIVE
    	if(Player)
    		Player->Hide();
    }
}

const FGameplayTag& UStateMachineComponent::GetLastTagTransitionExcluded() const
{
	if(!StateHistoric.Num() - 2 >= 0)
	{
		UE_LOGFMT(LogTemp, Warning, "EXIT IN GET LAST TAG - #{0}", PlayerIndex);
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

FVector UStateMachineComponent::GetPositionForState() const
{
	const FGameplayTag& PreviousTag = GetLastTagTransitionExcluded();
	UE_LOGFMT(LogTemp,Warning, "Previous Tag : {0} - #{1}", *PreviousTag.ToString(), PlayerIndex);

	if(CurrentActor)
		return CurrentActor->GetActorLocation() + FVector::UpVector * 10;
	
	/*if(PreviousTag == DA_StateMachine->InitState)
	{
		//Init
		return Player->GetActorLocation();
	}
	if(PreviousTag == DA_StateMachine->NeutralState)
	{
		//NEUTRAl
		return Player->GetActorLocation();
	}
	if (PreviousTag == DA_StateMachine->BallState)
	{
		//BALL
		return BallActor->GetActorLocation();
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
	if(PreviousTag == DA_StateMachine->ReviveState)
	{
		//REVIVE
		return Player->GetActorLocation();
	}*/
	checkf(nullptr, TEXT("No State in GetPositionState, last state : %s"), *CurrentState.GetName());
	return FVector();
}

FRotator UStateMachineComponent::GetRotationForState() const
{
	const FGameplayTag& PreviousTag = GetLastTagTransitionExcluded();
	UE_LOG(LogTemp, Warning, TEXT("Previous Tag : %s"), *PreviousTag.ToString());

	if (PreviousTag == DA_StateMachine->InitState)
	{
		//NEUTRAl
		return Player->GetActorRotation();
	}
	if (PreviousTag == DA_StateMachine->NeutralState)
	{
		//NEUTRAl
		return Player->GetActorRotation();
	}
	if (PreviousTag == DA_StateMachine->BallState)
	{
		//BALL
		return BallActor->GetActorRotation();
	}
	if (PreviousTag == DA_StateMachine->SpearState)
	{
		//SPEAR
		return SpearActor->GetActorRotation();
	}
	if (PreviousTag == DA_StateMachine->ShieldState)
	{
		//SHIELD
		return Player->GetActorRotation();
	}
	if (PreviousTag == DA_StateMachine->ReviveState)
	{
		//REVIVE
		return Player->GetActorRotation();
	}
	
	checkf(nullptr, TEXT("No State in GetRotationState, last state : %s"), *CurrentState.GetName());
	return FRotator();
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

		//TODO::Check hex eboulement
		if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params, FCollisionResponseParams()))
		{
			if(AHexBehaviour* Hex = Cast<AHexBehaviour>(HitResult.GetActor()))
			{
				PlayerLastHexPos = Hex->GetRespawnLoc();
			}
		}
	}
}

void UStateMachineComponent::IARandomMove()
{
	//UE_LOG(LogTemp, Warning, TEXT("IA RANDOM MOVE for player: %d"), PlayerIndex);
	if(!CurrentStateTag.MatchesTag(DA_StateMachine->NeutralState))
	{
		UE_LOG(LogTemp, Warning, TEXT("IA RANDOM MOVE FALSE : %s, DA : %s"), *CurrentStateTag.ToString(), *DA_StateMachine->NeutralState.ToString());
		return;
	}
	
	for(auto const IA : ArrayIA)
	{
		if(!IA) {continue;}

		if(CurrentActor)
		{
			const FVector halfSize = FVector(DA_StateMachine->DataAssetIA->RandomMoveDistanceFromPlayer, DA_StateMachine->DataAssetIA->RandomMoveDistanceFromPlayer,CurrentActor->GetActorLocation().Z);
			const FVector Destination = UKismetMathLibrary::RandomPointInBoundingBox(CurrentActor->GetActorLocation(), halfSize);
			if(IA)
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
		if (!IA) { continue; }

		FNavLocation NavLoc;
		const FRotator Rotation = IA->GetActorRotation();
		if(NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 300.f, NavLoc))
		{
			FVector TargetLoc = NavLoc.Location;
			IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
			IA->Show();
		}
		/*else
		{
			int count = 0;
			ShowAndTeleportIAFailed(IA, PlayerLoc, DestinationZ, count);
		}*/
	}
}

void UStateMachineComponent::ShowAndTeleportIAFailed(ACharacterBaseIA* IA, FVector PlayerLoc, float DestinationZ, int& count)
{
	count++;

	const FRotator Rotation = IA->GetActorRotation();

	if(count == 10)
	{
		UE_LOGFMT(LogTemp, Log, "show and teleport ia fail to find location");
		IA->SetActorLocationAndRotation(PlayerLoc, Rotation);
		IA->Show();
	}

	FNavLocation NavLoc;
	if(NavSystem->GetRandomReachablePointInRadius(PlayerLoc, 200.f, NavLoc))
	{
		FVector TargetLoc = NavLoc.Location;
		IA->SetActorLocationAndRotation(TargetLoc + (FVector::UpVector * DestinationZ), Rotation);
		IA->Show();
	}
	else
	{
		ShowAndTeleportIAFailed(IA, PlayerLoc, DestinationZ, count);
	}
}

void UStateMachineComponent::HideIA()
{
	for(auto IA : ArrayIA)
	{
		if(!IA) {continue;}

		IA->Hide();
	}
}
void UStateMachineComponent::DestroyIA()
{
	for(auto IA : ArrayIA)
	{
		if (!IA) { continue; }

		IA->OnSpiritDeath(PlayerIndex, IA->GetActorLocation());
		IA->Destroy();
	}
	ArrayIA.Empty();
}



