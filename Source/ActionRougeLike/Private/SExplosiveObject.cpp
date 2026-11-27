// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveObject::ASExplosiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;


	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(MeshComp);

	RadialForceComp->SetAutoActivate(false);//event component from ticking, as we don't need it to

	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ImpulseStrength = 2500.0f;

	RadialForceComp->bImpulseVelChange = true;

	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

}


void ASExplosiveObject::Explode()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Explode Barrel!"));

	RadialForceComp->FireImpulse();
	UE_LOG(LogTemp, Log, TEXT("ExplosiveObject exploded!"));
}

void ASExplosiveObject::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASExplosiveObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveObject::OnActorHit);
}


//void ASExplosiveObject::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	RadialForceComp->	();
//	UE_LOG(LogTemp, Log, TEXT("OnActorHit in ExplosiveObject"));
//}



// Called when the game starts or when spawned
void ASExplosiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASExplosiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

