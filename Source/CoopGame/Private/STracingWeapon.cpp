// Fill out your copyright notice in the Description page of Project Settings.


#include "STracingWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



void ASTracingWeapon::Fire()
{
    // trace from pawn eyes to crosshair

    AActor* MyOwner = GetOwner();

    if (MyOwner == nullptr)
        return;

    FVector EyeLocation;
    FRotator EyeRotation;
    MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    FVector ShootDirection = EyeRotation.Vector();

    FVector TraceEnd = EyeLocation + (ShootDirection * 10 * 1000);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MyOwner);
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = true;

    FVector TracerEndPoint = TraceEnd;

    FHitResult Hit;
    bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);

    if (bBlockingHit)
    {
        AActor* HitActor = Hit.GetActor();
        TracerEndPoint = Hit.ImpactPoint;

        UGameplayStatics::ApplyPointDamage(
            HitActor,
            DamageAmount,
            ShootDirection,
            Hit,
            MyOwner->GetInstigatorController(),
            this,
            DamageType
        );

        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
        }
    }

    if (TracerEffect)
    {
        FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
        UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

        if (TracerComp)
        {
            TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
        }
    }

    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
    }
}