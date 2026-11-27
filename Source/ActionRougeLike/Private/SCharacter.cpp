// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);


	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	PrimaryActorTick.bCanEverTick = true;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();	
	MoveComp->JumpZVelocity = 600.f;
	MoveComp->AirControl = 0.2f;	
	MoveComp->GravityScale = 1.5f;
	MoveComp->AirControlBoostMultiplier = 0.0f;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("IMC Added Successfully!"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: No PlayerController Possessing Character!"));
	}
}



void ASCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

void ASCharacter::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, FString::Printf(TEXT("Move Input: X=%f Y=%f"), MovementVector.X, MovementVector.Y));
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}


	if (Controller != nullptr) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//void ASCharacter::Look(const FInputActionValue& Value) {
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//	// X = Forward (Red)
//	// Y = Right (Green)
//	// Z = Up (Blue)
//
//	if (Controller != nullptr) {
//		AddControllerYawInput(LookAxisVector.X);
//		AddControllerPitchInput(-1 * LookAxisVector.Y);
//	}
//}

void ASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		// Player actively moved the mouse: allow the short idle-turn timer to align the body
		if (!LookAxisVector.IsNearlyZero())
		{
			bWantsIdleTurn = true;
			IdleLookTimer = 0.0f;
		}
	}
}

void ASCharacter::Shoot(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Shoot action triggered!"));

	FVector HandLocation = GetMesh()->GetSocketLocation("Left_Pincer_Arm_2");

	// Use GetControlRotation() to aim where the camera is looking, flattened horizontally
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FTransform SpawnTM = FTransform(ControlRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}


//void ASCharacter::Shoot(const FInputActionValue& Value)
//{
//	if (ProjectileClass == nullptr)
//	{
//		return;
//	}
//
//	// Use camera look direction (GetControlRotation) instead of flat GetActorRotation
//	const FRotator AimRotation = GetControlRotation();
//	const FVector SpawnLocation = GetActorLocation() + (AimRotation.Vector() * 100.0f);
//	const FTransform SpawnTM(AimRotation, SpawnLocation);
//
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnParams.Instigator = this;
//
//	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
//}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (Controller == nullptr || MoveComp == nullptr)
	{
		return;
	}

	bUseControllerRotationYaw = false;
	MoveComp->bOrientRotationToMovement = false;

	const FRotator CurrentRot = GetActorRotation();
	const float ControlYaw = Controller->GetControlRotation().Yaw;
	const float DeltaYaw = FRotator::NormalizeAxis(ControlYaw - CurrentRot.Yaw);
	const float AbsDeltaYaw = FMath::Abs(DeltaYaw);
	const bool bIsMoving = GetVelocity().SizeSquared() > 10.f;

	if (bIsMoving)
	{
		IdleLookTimer = 0.0f;
		bWantsIdleTurn = false;
		bWasMoving = true;

		// Smoothly turn toward the walking direction (W, A, S, D)
		const float WalkYaw = GetVelocity().Rotation().Yaw;
		const FRotator TargetRot(0.f, WalkYaw, 0.f);
		SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 10.f));
	}
	else
	{
		// On the exact frame we stop walking, lock the ±45° grace cone around the current camera direction
		if (bWasMoving)
		{
			StoppedCameraYaw = ControlYaw;
			bWasMoving = false;
		}

		if (bWantsIdleTurn)
		{
			// How far the camera has moved from where it was when we stopped walking
			const float CameraDriftFromStop = FMath::Abs(FRotator::NormalizeAxis(ControlYaw - StoppedCameraYaw));

			if (CameraDriftFromStop > 45.f)
			{
				// Broke out of the ±45° camera grace cone: smoothly turn ALL THE WAY to the camera direction without pausing
				const FRotator TargetRot(0.f, ControlYaw, 0.f);
				SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 3.5f));

				// Once aligned with the camera, update the grace anchor so you get a fresh ±45° cone here
				if (AbsDeltaYaw <= 2.f)
				{
					StoppedCameraYaw = ControlYaw;
					bWantsIdleTurn = false;
					IdleLookTimer = 0.0f;
				}
			}
			else if (AbsDeltaYaw > 2.f && AbsDeltaYaw <= 45.f)
			{
				// Character and camera are already facing the same general direction (±45°):
				// Wait 0.4s, then gently square up with the camera
				IdleLookTimer += DeltaTime;
				if (IdleLookTimer >= 0.4f)
				{
					const FRotator TargetRot(0.f, ControlYaw, 0.f);
					SetActorRotation(FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 3.5f));
				}
			}
			else
			{
				// Inside the ±45° grace cone while facing sideways or backward: hold pose
				IdleLookTimer = 0.0f;
				bWantsIdleTurn = false;
			}
		}
	}
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Inside Movement"));
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		EnhancedInput->BindAction(ShootAction, ETriggerEvent::Started, this, &ASCharacter::Shoot);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}


