// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"


// Sets default values
ASWeapon::ASWeapon()
{
    DamageAmount = 20.f;
    FireRate = 1.f;


    MuzzleSocketName = "MuzzleSocket";
    TracerTargetName = "Target";

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");

    RootComponent = MeshComponent;

    LastFiredTime = 0.f;
}


void ASWeapon::Fire()
{
    LastFiredTime = GetWorld()->TimeSeconds;
}

void ASWeapon::StartFire()
{
    // shots per second
    const float FireDelay = 1.f / FireRate;
    const float FirstDelay = FMath::Max(0.f, LastFiredTime + FireDelay - GetWorld()->TimeSeconds);
    GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, FireDelay, true, FirstDelay);
}

void ASWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}
