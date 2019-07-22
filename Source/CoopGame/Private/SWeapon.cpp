// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASWeapon::ASWeapon()
{
    DamageAmount = 20.f;

    MuzzleSocketName = "MuzzleSocket";
    TracerTargetName = "Target";

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");

    RootComponent = MeshComponent;
}


void ASWeapon::Fire()
{

}
