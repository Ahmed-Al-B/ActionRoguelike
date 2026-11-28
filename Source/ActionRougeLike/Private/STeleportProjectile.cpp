// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"
//#include ""

// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComp->InitialSpeed = InitialSpeed;

	NiagaraCompStart = CreateDefaultSubobject<UNiagaraComponent>("NiagaraCompStart");
	NiagaraCompStart->SetupAttachment(SphereComp);
	NiagaraCompStart->SetVisibility(false);


	NiagaraCompEnd = CreateDefaultSubobject<UNiagaraComponent>("NiagaraCompEnd");
	NiagaraCompEnd->SetupAttachment(SphereComp);
	NiagaraCompEnd->SetVisibility(false);



}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayAction, this, &ASTeleportProjectile::StopMovement, TravelTime);
}

void ASTeleportProjectile::StopMovement() {

	GetWorldTimerManager().ClearTimer(TimerHandle_DelayAction);


	MovementComp->StopMovementImmediately();
	SphereComp->SetSimulatePhysics(false);
	EffectComp->SetVisibility(false);

	NiagaraCompStart->SetVisibility(true);
	NiagaraCompStart->Activate(true);

	GetWorldTimerManager().SetTimer(TimerHandle_DelayAction, this, &ASTeleportProjectile::MovePlayer, EffectDuration);

}


void ASTeleportProjectile::MovePlayer()
{
	AActor* MoveActor = GetInstigator();

	if (MoveActor) {
		//FVector ObjectLocation = GetActorLocation();
		FVector Destination = bHasSafeLocation ? SafeLocation : GetActorLocation();
		MoveActor->TeleportTo(Destination, MoveActor->GetActorRotation());
		NiagaraCompStart->SetVisibility(true);
	}
	//i could create another moveactor just to check the getinstigator inside the other classes to check first

	NiagaraCompStart->Deactivate();
	NiagaraCompEnd->SetVisibility(true);
	NiagaraCompEnd->Activate(true);

	//GetWorldTimerManager().SetTimer(TimerHandle_DelayAction, this, &ASTeleportProjectile::DestroySelf, EndEffectDuration);
	SetLifeSpan(EndEffectDuration);
	//Destroy();

}



void ASTeleportProjectile::DestroySelf()
{
	Destroy();
}

// Called every frame
void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASTeleportProjectile::PostInitializeComponents() {
	Super::PostInitializeComponents();

	//if (GetInstigator()) {
	//	SphereCom
	//}

	SphereComp->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::OnActorHit);
}

void ASTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		// Use 90.0f (typical character capsule half-height) so floor hits clear the ground cleanly
		SafeLocation = Hit.ImpactPoint + (Hit.ImpactNormal * 50.0f);
		bHasSafeLocation = true;

		StopMovement();
	}
}



