// Fill out your copyright notice in the Description page of Project Settings.


#include "STracingWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "CoopGame.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"


static int32 DebugWeaponDrawing = 0;

FAutoConsoleVariableRef CVARDebugWeaponDrawing(
    TEXT("COOP.DebugWeapons"),
    DebugWeaponDrawing,
    TEXT("Draw Debug Lines for Weapons"),
    ECVF_Cheat
);




void ASTracingWeapon::Fire()
{
	Super::Fire();
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	UE_LOG(LogTemp, Log, TEXT("Tracing fire"));

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
    QueryParams.bReturnPhysicalMaterial = true;

    FVector TracerEndPoint = TraceEnd;
	EPhysicalSurface SurfaceType = SurfaceType_Default;
	
    FHitResult Hit;
    bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        EyeLocation,
        TraceEnd,
        COLLISION_WEAPON,
        QueryParams
    );

    if (bBlockingHit)
    {
        AActor* HitActor = Hit.GetActor();
        TracerEndPoint = Hit.ImpactPoint;

        float ActualDamage = DamageAmount;

        SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

        if (SurfaceType == SURFACE__FLESH_VULNERABLE)
        {
            ActualDamage *= 3.f;
        }

        UGameplayStatics::ApplyPointDamage(
            HitActor,
            ActualDamage,
            ShootDirection,
            Hit,
            MyOwner->GetInstigatorController(),
            this,
            DamageType
        );

		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
		
    }

    if (DebugWeaponDrawing > 0)
    {
        DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
    }

    PlayFireEffects(TracerEndPoint);

	if (Role == ROLE_Authority)
	{
		HitScanTrace.TraceTo = TracerEndPoint;
		HitScanTrace.SurfaceType = SurfaceType;
	}	
}


void ASTracingWeapon::PlayFireEffects(const FVector TracerEndPoint)
{
    if (MuzzleEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
    }

    if (TracerEffect)
    {
	    const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

        UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            TracerEffect,
            MuzzleLocation
        );

        if (TracerComp)
        {
            TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
        }
    }

    APawn* MyOwner = Cast<APawn>(GetOwner());
    if (MyOwner)
    {
        APlayerController* Controller = Cast<APlayerController>(MyOwner->GetController());
        if (Controller && FireCamShake)
        {
            Controller->ClientPlayCameraShake(FireCamShake);
        }
    }
}

void ASTracingWeapon::PlayImpactEffects(const EPhysicalSurface SurfaceType, const FVector ImpactPoint)
{

	UParticleSystem* SelectedEffect = DefaultImpactEffect;
	switch (SurfaceType)
	{
	case SURFACE__FLESH_DEFAULT:
		SelectedEffect = FleshImpactEffect;
		break;
	case SURFACE__FLESH_VULNERABLE:
		SelectedEffect = VulnerableImpactEffect;
		break;
	default:
		break;
	}

	if (SelectedEffect)
	{
		FVector ShotDirection = ImpactPoint - MeshComponent->GetSocketLocation(MuzzleSocketName);
		ShotDirection.Normalize();
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ASTracingWeapon::OnRep_HitScanTrace()
{
	// Play Cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}



void ASTracingWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASTracingWeapon, HitScanTrace, COND_SkipOwner);
}