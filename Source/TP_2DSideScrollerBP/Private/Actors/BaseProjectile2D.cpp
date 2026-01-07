#include "Actors/BaseProjectile2D.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile2D::ABaseProjectile2D()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);

	CollisionComp->SetCollisionProfileName("BlockAllDynamic");

	CollisionComp->SetNotifyRigidBodyCollision(true);

	RootComponent = CollisionComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	SpriteComp->SetupAttachment(RootComponent);
	SpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;

	ProjectileMovement->InitialSpeed = 600.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));
}

void ABaseProjectile2D::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CollisionComp))
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile2D::OnHit);
	}

	// Логування для перевірки життя снаряда
	UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned: %s"), *GetName());
}

void ABaseProjectile2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProjectileMovement)
	{
		float Speed = ProjectileMovement->Velocity.Size();
		if (Speed < 10.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("Projectile STOPPED or TOO SLOW"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Speed: %f"), Speed));
		}
	}
}

void ABaseProjectile2D::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection.GetSafeNormal() * ProjectileMovement->InitialSpeed;
}

void ABaseProjectile2D::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("BAM! Hit Actor: %s"), *OtherActor->GetName());

	if ((IsValid(OtherActor)) && (OtherActor != this))
	{
		//Destroy(); // Поки закоментовано для тесту
	}
}