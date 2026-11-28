// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGamerInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void USInteractionComponent::PrimaryInteract()
//{
//	// Log a debug message to the output log
//	UE_LOG(LogTemp, Log, TEXT("Interacting"));
//
//	FCollisionObjectQueryParams ObjectQueryParams;	
//	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
//
//	AActor* MyOwner = GetOwner();
//
//	if (MyOwner == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Interaction failed: Owner is null."));
//		return;
//	}
//
//	
//
//	FVector EyeLocation;	
//	FRotator EyeRotation;
//	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
//
//	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);
//
//	//FHitResult Hit;
//	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
//	
//	FCollisionShape Shape;
//	Shape.SetSphere(30.f);
//
//	TArray<FHitResult> Hits;
//
//	bool bSweepSuccess = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
//	
//	FColor LineColor = bSweepSuccess ? FColor::Green : FColor::Red;
//	//boolean found = false;
//	for (FHitResult Hit : Hits) {
//
//		AActor* HitActor = Hit.GetActor();
//		if (HitActor) {
//			if (HitActor->Implements<USGamerInterface>()) {
//				// Handle the interaction
//				APawn* MyPawn = Cast<APawn>(MyOwner);
//				ISGamerInterface::Execute_Interact(HitActor, MyPawn);
//				break;
//			}
//
//		}
//		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 32, LineColor, false, 2.0f);
//	}
//
//
//}

void USInteractionComponent::PrimaryInteract()
{
	UE_LOG(LogTemp, Log, TEXT("Interacting"));

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction failed: Owner is null."));
		return;
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 3000.0f);

	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	TArray<FHitResult> Hits;
	bool bSweepSuccess = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape, QueryParams);

	FColor LineColor = bSweepSuccess ? FColor::Green : FColor::Red;

	for (const FHitResult& Hit : Hits)
	{
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.0f, 32, LineColor, false, 2.0f);

		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGamerInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGamerInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}
