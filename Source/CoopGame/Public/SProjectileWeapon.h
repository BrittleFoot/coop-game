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


protected:

    virtual void Fire() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon (Projectile)")
    TSubclassOf<ASProjectile> ProjectileClass;

    UPROPERTY(VisibleAnywhere, Category = "Weapon (Projectile)")
    float LaunchPower;

};