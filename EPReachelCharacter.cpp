// Fill out your copyright notice in the Description page of Project Settings.


#include "EPReachelCharacter.h"
#include "ProyectilPrincipal.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"


AEPReachelCharacter::AEPReachelCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.0f);

	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	ReachelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ReachelMesh"));
	ReachelMesh->SetupAttachment(GetCapsuleComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;

	ArmOffsetRight = FVector(0.0f, 0.0f, 0.0f); //100.0f, 0.0f, 10.0f )( 65.0f, -22.0f, -24.0f
	ArmOffsetLeft = FVector(0.0f, 0.0f, 0.0f); //0.0f, 0.0f, 0.0f

	FireMuzzleLocation = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMuzzleLocation"));
	FireMuzzleLocation->SetupAttachment(ReachelMesh);
	FireMuzzleLocation->SetupAttachment(ReachelMesh, TEXT("FirePower_Socket_R"));
	FireMuzzleLocation->SetRelativeLocation(FVector(0, 0, 0));
	
	FireMuzzleLocationLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FireMuzzleLocationLeft"));
	FireMuzzleLocationLeft->SetupAttachment(ReachelMesh);
	FireMuzzleLocationLeft->SetupAttachment(ReachelMesh, TEXT("FirePower_Socket_L"));
	FireMuzzleLocationLeft->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FireMuzzleLocationLeft->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void AEPReachelCharacter::BeginPlay()
{
	Super::BeginPlay();

	RightFire = true;
}

void AEPReachelCharacter::OnFire()
{
	if(RightFire == true)
	{
		OnFireRight();
		GetWorldTimerManager().SetTimer(TimerHandle_RightFire, this, &AEPReachelCharacter::OnFireRight, 10.0f, true);
		LeftFire = true;
	}

	else if(LeftFire == true)
	{
		OnFireLeft();
		GetWorldTimerManager().SetTimer(TimerHandle_LeftFire, this, &AEPReachelCharacter::OnFireLeft, 10.0f, true);
		RightFire = true;
	}
}

void AEPReachelCharacter::OnFireRight()
{
	if (FireProyectile != NULL && RightFire == true && LeftFire == false)
	{
		RightFire = false;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FRotator NuevoR = FRotator(0, -0.4f, 0);
			FRotator SpawnRotation = GetViewRotation();
			FVector SpawnLocation = ((FireMuzzleLocation != nullptr) ? FireMuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(ArmOffsetRight);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AProyectilPrincipal>(FireProyectile, SpawnLocation, SpawnRotation + NuevoR, ActorSpawnParams);
		}

		if (FireSoundRight != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSoundRight, GetActorLocation(), 0.2f);
		}

		if (FireAnimationRight != NULL)
		{
			UAnimInstance* AnimInstance = ReachelMesh->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimationRight, 1.f);
			}
		}
		GetWorldTimerManager().ClearTimer(TimerHandle_RightFire);
	}
}

void AEPReachelCharacter::OnFireLeft()
{
	if (FireProyectileLeft != NULL && LeftFire == true && RightFire == false)
	{
		LeftFire = false;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FRotator NuevoL = FRotator(0, 0.4f, 0);
			FRotator SpawnRotation = GetViewRotation();
			FVector SpawnLocation = ((FireMuzzleLocationLeft != nullptr) ? FireMuzzleLocationLeft->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(ArmOffsetLeft);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<AProyectilPrincipal>(FireProyectileLeft, SpawnLocation, SpawnRotation + NuevoL, ActorSpawnParams);
		}

		if (FireSoundLeft != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSoundLeft, GetActorLocation(), 0.2f);
		}

		if (FireAnimationLeft != NULL)
		{
			
			UAnimInstance* AnimInstance = ReachelMesh->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimationLeft, 1.f);
			}
		}
		GetWorldTimerManager().ClearTimer(TimerHandle_LeftFire);
	}
}

// Called every frame
void AEPReachelCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if()
	{
	}*/
}

// Called to bind functionality to input
void AEPReachelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEPReachelCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEPReachelCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AEPReachelCharacter::OnFire);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEPReachelCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEPReachelCharacter::MoveRight);
}

void AEPReachelCharacter::MoveForward(float Axis)
{
	if((Controller != NULL) && (Axis != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AEPReachelCharacter::MoveRight(float Axis)
{
	if((Controller != NULL) && (Axis != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void AEPReachelCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEPReachelCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FRotator AEPReachelCharacter::GetViewRotation() const
{
	if (Controller)
	{
		return Controller->GetControlRotation();
	}

	return FRotator(RemoteViewPitch / 255.f * 360.f, GetActorRotation().Yaw, 0.f);
}
