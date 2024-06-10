#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DABall.generated.h"


UCLASS()
class COOPIA_API UDABall : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float launchForceCanon = 10000.f;

	UPROPERTY(EditAnywhere)
	float maxVelocityCanon = 1600.f;

	UPROPERTY(EditAnywhere)
	float loadTimeCanon = 2.f;
};