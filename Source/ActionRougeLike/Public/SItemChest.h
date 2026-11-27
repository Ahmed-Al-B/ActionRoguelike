// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGamerInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;


UCLASS()
class ACTIONROUGELIKE_API ASItemChest : public AActor, public ISGamerInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Sets default values for this actor's properties

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bLidOpen;

	void UpdateLidRotation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
