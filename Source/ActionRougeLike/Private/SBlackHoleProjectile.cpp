// Fill out your copyright notice in the Description page of Project Settings.

#include "SBlackHoleProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	MovementComp->InitialSpeed = 1800.0f;

	SphereComp->SetSphereRadius(25.0f);

	// 1. Set base profile FIRST, then override channels so they aren't wiped out
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 2. Overlap everything by default so the black hole passes through objects smoothly, but ignore Pawns
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	// 3. Setup Radial Force (starts inactive until ForceStartDelay finishes)
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(SphereComp);
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ForceStrength = 0.0f;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void ASBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHoleProjectile::OnActorOverlap);
}

void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Automatically destroys this Black Hole actor after 5.0 seconds
	//SetLifeSpan(5.0f);

	//GetWorldTimerManager().SetTimer(TimerHandle_ActivateForce, this, &ASBlackHoleProjectile::ActivateSuctionForce, ForceStartDelay);
}

void ASBlackHoleProjectile::ActivateSuctionForce()
{
	// Ensure TargetForceStrength is negative (e.g., -2000000.0f) in your header/Blueprint to pull inward
	RadialForceComp->ForceStrength = TargetForceStrength;
	RadialForceComp->Activate(true);
}

void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherActor->Destroy();
		}
	}
}
