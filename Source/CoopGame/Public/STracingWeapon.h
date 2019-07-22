// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "STracingWeapon.generated.h"

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

};
