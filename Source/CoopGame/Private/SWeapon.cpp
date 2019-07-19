// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASWeapon::ASWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

    DamageAmount = 20.f;

    MuzzleSocketName = "MuzzleSocket";
    TracerTargetName = "Target";

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");

    RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASWeapon::Fire()
{

}
