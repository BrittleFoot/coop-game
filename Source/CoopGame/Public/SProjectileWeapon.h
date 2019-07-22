// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SProjectileWeapon.generated.h"


class ASProjectile;


/**
 *
 */
UCLASS()
class COOPGAME_API ASProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()


public:

    virtual void Fire() override;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon (Projectile)")
    TSubclassOf<ASProjectile> ProjectileClass;

    UPROPERTY(VisibleAnywhere, Category = "Weapon (Projectile)")
    float LaunchPower;

    void PlayFireEffects();

};
