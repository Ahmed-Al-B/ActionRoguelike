// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackHoleProjectile.generated.h"


class URadialForceComponent;

UCLASS()
class ACTIONROUGELIKE_API ASBlackHoleProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASBlackHoleProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	URadialForceComponent* RadialForceComp;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_ActivateForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BlackHole")
	float TargetForceStrength = -2000000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "BlackHole")
	float ForceStartDelay = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlackHole")
	float LifeSpan = 5.0f;

	//void ActivateSuctionForce();

	//UFUNCTION()
	//void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ActivateSuctionForce();


	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};