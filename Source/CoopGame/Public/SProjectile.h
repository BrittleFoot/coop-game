// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class UParticleSystem;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class COOPGAME_API ASProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float BaseDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float DamageRadius;

public:

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void Explode(AActor* Actor);

};
