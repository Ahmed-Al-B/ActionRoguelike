// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"


//void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
//{
//
//	//LidMesh->SetRelativeRotation(FRotator(this->TargetPitch, 0.f, 0.f));
//	
//	bLidOpen = !bLidOpen;
//
//	float NewPitch = bLidOpen ? TargetPitch : 0.0f;
//	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
//
//
//}

void ASItemChest::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateLidRotation();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpen = !bLidOpen;
	UpdateLidRotation();
}

void ASItemChest::UpdateLidRotation()
{
	const float NewPitch = bLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->TargetPitch = 110.f;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");	
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");	
	LidMesh->SetupAttachment(BaseMesh);

	bLidOpen = false;

}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

