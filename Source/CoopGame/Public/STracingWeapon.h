// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "STracingWeapon.generated.h"


// info about of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};


/**
 *
 */
UCLASS()
class COOPGAME_API ASTracingWeapon : public ASWeapon
{
	GENERATED_BODY()

public:

	virtual void Fire() override;

protected:

	void PlayFireEffects(const FVector TracerEndPoint);

	void PlayImpactEffects(const EPhysicalSurface SurfaceType, const FVector ImpactPoint);

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
