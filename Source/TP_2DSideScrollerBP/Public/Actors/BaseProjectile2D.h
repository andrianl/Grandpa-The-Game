// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile2D.generated.h"

class USphereComponent;
class UPaperSpriteComponent;
class UProjectileMovementComponent;

UCLASS()
class TP_2DSIDESCROLLERBP_API ABaseProjectile2D : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile2D();

		UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& ShootDirection);

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	UPaperSpriteComponent* SpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

};
