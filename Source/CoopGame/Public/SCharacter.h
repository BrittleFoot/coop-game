// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASWeapon;


UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight(float Value);

	void MoveForward(float Value);

	void BeginCrouch();

	void EndCrouch();

	void DoJump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

    UFUNCTION(BlueprintImplementableEvent, Category = "Character")
    bool OnPickup(AActor* Actor);


    bool bWantsToZoom;

    UPROPERTY(EditDefaultsOnly, Category = "Player")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
    float ZoomInterpSpeed;

    float DefaultFOV;

    void BeginZoom();

    void EndZoom();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    ASWeapon* CurrentWeapon;

    UFUNCTION(BlueprintCallable, Category = "Character")
    void StartFire();

    UFUNCTION(BlueprintCallable, Category = "Character")
    void StopFire();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual FVector GetPawnViewLocation() const override;

    UFUNCTION(BlueprintCallable, Category = "Character")
    void Pickup(AActor* Actor);

};
