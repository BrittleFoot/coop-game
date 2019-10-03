// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupItem.generated.h"


class USkeletalMeshComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;
class UMaterialInterface;


UCLASS()
class COOPGAME_API ASPickupItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPickupItem();

protected:

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Replicated, Category = "PickupItem")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = "PickupItem")
	AActor* Item;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* DisplayMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated, Category = "Components")
	USkeletalMeshComponent* DisplaySkeletalMeshComponent;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WrapperComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	URotatingMovementComponent* RotatingMovementComponent;

	UFUNCTION()
	void HandleBeingOverlapped(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void InitializeDisplayItem();

public:

	UFUNCTION(BlueprintCallable, Category = "PickupItem")
	void SetItem(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "PickupItem")
	AActor* Peek() const;

	UFUNCTION(BlueprintCallable, Category = "PickupItem")
	AActor* Pickup();
};
