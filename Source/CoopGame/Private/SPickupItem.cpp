// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPickupItem::ASPickupItem()
{
	Item = nullptr;
	ItemClass = nullptr;

	WrapperComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WrapperComponent"));
	WrapperComponent->SetGenerateOverlapEvents(true);
	WrapperComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DisplayMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMeshComponent"));
	DisplayMeshComponent->SetHiddenInGame(true);
	DisplayMeshComponent->SetupAttachment(WrapperComponent);
	DisplayMeshComponent->SetGenerateOverlapEvents(false);
	DisplayMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DisplaySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DisplaySkeletalMeshComponent"));
	DisplaySkeletalMeshComponent->SetHiddenInGame(true);
	DisplaySkeletalMeshComponent->SetupAttachment(WrapperComponent);
	DisplaySkeletalMeshComponent->SetGenerateOverlapEvents(false);
	DisplaySkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	RootComponent = WrapperComponent;

	SetReplicates(true);
}

void ASPickupItem::HandleBeingOverlapped(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ASCharacter* Player = Cast<ASCharacter>(OtherActor);
	if (Player)
	{
		Player->Pickup(this);
	}
}

// Called when the game starts or when spawned
void ASPickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (DisplaySkeletalMeshComponent)
	{
		DisplaySkeletalMeshComponent->AddLocalRotation(FRotator(0.f, -90.f, -30.f));
	}

	if (ItemClass && Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* Actor = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnParameters);

		SetItem(Actor);
	}

	InitializeDisplayItem();
	
	WrapperComponent->OnComponentBeginOverlap.AddDynamic(this, &ASPickupItem::HandleBeingOverlapped);
}

void ASPickupItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickupItem, Item);
	DOREPLIFETIME(ASPickupItem, ItemClass);
	DOREPLIFETIME(ASPickupItem, DisplaySkeletalMeshComponent);
}

void ASPickupItem::InitializeDisplayItem()
{
	if (!Item)
	{
		return;
	}
	
	USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(
		Item->GetComponentByClass(USkeletalMeshComponent::StaticClass())
	);

	if (SkeletalMeshComponent)
	{
		DisplaySkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshComponent->SkeletalMesh);
		DisplaySkeletalMeshComponent->SetHiddenInGame(false);
	}
	else
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(
			Item->GetComponentByClass(UStaticMeshComponent::StaticClass())
		);

		if (StaticMeshComponent)
		{
			DisplayMeshComponent->SetStaticMesh(StaticMeshComponent->GetStaticMesh());
			DisplayMeshComponent->SetHiddenInGame(false);
		}
	}
}

void ASPickupItem::SetItem(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	Item = Actor;
	ItemClass = Actor->GetClass();

	if (Item)
	{
		Item->SetActorHiddenInGame(true);
		Item->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

AActor* ASPickupItem::Peek() const
{
	return Item;
}

AActor* ASPickupItem::Pickup()
{
	Destroy();
	Item->SetActorHiddenInGame(false);
	Item->DetachFromActor(FDetachmentTransformRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepRelative,
		EDetachmentRule::KeepRelative,
		true
	));

	return Item;
}
