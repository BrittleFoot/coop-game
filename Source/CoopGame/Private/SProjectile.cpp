// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

#include "SWeapon.h"


static int32 DebugProjectileDrawing = 0;

FAutoConsoleVariableRef CVARDebugProjectileDrawing(
    TEXT("COOP.DebugProjectiles"),
    DebugProjectileDrawing,
    TEXT("Draw Debug Spheres for Projectiles"),
    ECVF_Cheat
);



// Sets default values
ASProjectile::ASProjectile()
{

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(15.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComponent->CanCharacterStepUpOn = ECB_No;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3;
    ProjectileMovement->Friction = 0.3;

    BaseDamage = 100.f;
    DamageRadius = 150.f;

    InitialLifeSpan = 1.0f;
    RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
    OnDestroyed.AddDynamic(this, &ASProjectile::Explode);
}


void ASProjectile::Explode(AActor* Actor)
{
    ASWeapon* WeaponOwner = Cast<ASWeapon>(GetOwner());
    if (WeaponOwner == nullptr)
        return;


    TArray<AActor*> IgnoreActors;
    UGameplayStatics::ApplyRadialDamage(
        this,
        BaseDamage,
        GetActorLocation(),
        DamageRadius,
        WeaponOwner->DamageType,
        IgnoreActors,
        this,
        Instigator->GetInstigatorController()
    );

    if (DebugProjectileDrawing > 0)
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Cyan, false,  1.f);
    }

    if (ExplosionEffect)
    {
        UParticleSystemComponent* ExplosionComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    }

    Destroy();
}
