// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Templates/SubclassOf.h"
#include "SCharacter.generated.h"
	
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	TSubclassOf<AActor> ProjectileClass;



public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	FTimerHandle TimerHandle_Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	float ShootDelay = 0.25f;

	

	float IdleLookTimer = 0.0f;
	float StoppedCameraYaw = 0.0f;
	bool bWantsIdleTurn = false;
	bool bWasMoving = false;

	void MoveForward(float Value);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Shoot_TimeElapsed();
	void Enteract(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ShootAnim;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
