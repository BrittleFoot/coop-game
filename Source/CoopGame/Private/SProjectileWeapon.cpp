// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "SProjectile.h"



void ASProjectileWeapon::Fire()
{
    // trace from pawn eyes to crosshair

    APawn* MyOwner = Cast<APawn>(GetOwner());

    if (MyOwner == nullptr)
        return;


    FVector EyeLocation;
    FRotator EyeRotation;
    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
    EyeRotation.Pitch += 5.f;

    FVector ShootDirection = EyeRotation.Vector();
    FVector SpawnLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = true;

    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    ActorSpawnParams.Instigator = MyOwner;
    ActorSpawnParams.Owner = this;

    GetWorld()->SpawnActor<ASProjectile>(ProjectileClass, SpawnLocation, EyeRotation, ActorSpawnParams);


    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
    }
}
