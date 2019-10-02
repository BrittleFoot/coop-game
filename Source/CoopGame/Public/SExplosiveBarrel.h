// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class USHealthComponent;
class UMaterial;
class UParticleSystem;
class UDamageType;
class UMovementComponent;


UCLASS()
class COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterial* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(BlueprintReadOnly, Category = "Barrel")
	bool bExploded;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* ExplosionEffect;

	UFUNCTION()
	void AcceptDamage(USHealthComponent* OwnedHealthComponent,
	                  float Health,
	                  float HealthDelta,
	                  const class UDamageType* AppliedDamageType,
	                  class AController* InstigatedBy,
	                  AActor* DamageCauser);

	void Explode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
