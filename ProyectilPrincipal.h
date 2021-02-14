// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ProyectilPrincipal.generated.h"

UCLASS(config = Game)
class ETERNALPOWER_API AProyectilPrincipal : public AActor
{
	GENERATED_BODY()


public:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category = Particles)
		UParticleSystemComponent* ProjectileParticles;

public:	
	// Sets default values for this actor's properties
	AProyectilPrincipal();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
