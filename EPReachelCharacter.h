// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EPReachelCharacter.generated.h"

class UInputComponet;

UCLASS(config = Game)
class ETERNALPOWER_API AEPReachelCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	AEPReachelCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* ReachelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FireMuzzleLocation;
		
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FireMuzzleLocationLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UAnimMontage* FireAnimationRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UAnimMontage* FireAnimationLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSoundRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSoundLeft;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProyectilPrincipal> FireProyectile;
		
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProyectilPrincipal> FireProyectileLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector ArmOffsetRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector ArmOffsetLeft;
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TimeBetweenShotFR;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool RightFire;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TimeBetweenShotFL;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool LeftFire;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnFire();

	void OnFireRight();
	
	void OnFireLeft();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FTimerHandle TimerHandle_RightFire;
	FTimerHandle TimerHandle_LeftFire;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FRotator GetViewRotation() const override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
