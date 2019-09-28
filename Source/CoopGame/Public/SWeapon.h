// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
struct FTimerHandle;


UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASWeapon();

protected:


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = 0.01))
    float FireRate;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName MuzzleSocketName;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName TracerTargetName;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* MuzzleEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* DefaultImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* FleshImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* VulnerableImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    UParticleSystem* TracerEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<UCameraShake> FireCamShake;

    UFUNCTION(Category = "Weapon")
    virtual void Fire();

    FTimerHandle TimerHandle_TimeBetweenShots;

    float LastFiredTime;

public:

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void StopFire();


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UDamageType> DamageType;

};
