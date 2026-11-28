// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SProjectileBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STeleportProjectile.generated.h"

//class FTimerHandle;
class UNiagaraComponent;


UCLASS()
class ACTIONROUGELIKE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle_DelayAction;

	void StopMovement();
	//void GenerateEffects();
	void MovePlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport");
	float TravelTime = .25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport");
	float EffectDuration = .25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Teleport");
	float InitialSpeed = 4000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float EndEffectDuration = 0.5f;

	UFUNCTION()
	void DestroySelf();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraCompStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraCompEnd;

	FVector SafeLocation = FVector::ZeroVector;
	bool bHasSafeLocation = false;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
