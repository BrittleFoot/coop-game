// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SHealthComponent.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/MovementComponent.h"



// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MeshComponent;

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	
	ExplodedMaterial = CreateDefaultSubobject<UMaterial>(TEXT("ExplodedBarrelMaterial"));

	DamageRadius = 300.f;
	BaseDamage = 150.f;

}

void ASExplosiveBarrel::AcceptDamage(USHealthComponent* OwnedHealthComponent,
	float Health,
	float HealthDelta,
	const UDamageType* AppliedDamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Health <= 0.0f && !bExploded)
	{
		bExploded = true;
		Explode();
	}
}

void ASExplosiveBarrel::Explode()
{
	UE_LOG(LogTemp, Log, TEXT("Barrel Exploded"));

	MeshComponent->SetMaterial(0, ExplodedMaterial);


	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		this,
		BaseDamage,
		GetActorLocation(),
		DamageRadius,
		DamageType,
		IgnoreActors,
		this,
		nullptr
	);

	MeshComponent->AddImpulse(FVector(0.f, 0.f, BaseDamage + DamageRadius), NAME_None, true);

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::AcceptDamage);
}


