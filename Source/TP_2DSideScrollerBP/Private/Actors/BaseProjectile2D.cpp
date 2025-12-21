#include "Actors/BaseProjectile2D.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile2D::ABaseProjectile2D()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f); // «меншив рад≥ус (40 см це €к футбольний м'€ч, забагато дл€ кул≥)

	// ¬ј∆Ћ»¬ќ: ƒл€ тесту ставимо BlockAllDynamic, щоб точно вдар€тис€
	// ѕ≥зн≥ше повернете на "Projectile", коли налаштуЇте Presets
	CollisionComp->SetCollisionProfileName("BlockAllDynamic");

	// ¬микаЇмо генерац≥ю под≥й удару
	CollisionComp->SetNotifyRigidBodyCollision(true);

	RootComponent = CollisionComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	SpriteComp->SetupAttachment(RootComponent);
	SpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;

	// «б≥льшуЇмо швидк≥сть!
	ProjectileMovement->InitialSpeed = 600.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// ¬имикаЇмо грав≥тац≥ю, щоб лет≥в пр€мо
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f)); // ЅлокуЇмо Y
}

void ABaseProjectile2D::BeginPlay()
{
	// --- ¬»ѕ–ј¬Ћ≈ЌЌя 1: ќбов'€зково викликаЇмо батьк≥вський клас ---
	Super::BeginPlay();

	if (IsValid(CollisionComp))
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile2D::OnHit);
	}

	// Ћогуванн€ дл€ перев≥рки житт€ снар€да
	UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned: %s"), *GetName());
}

void ABaseProjectile2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// якщо швидк≥сть > 0, ви це побачите
	if (ProjectileMovement)
	{
		// якщо швидк≥сть дуже мала, виведемо попередженн€
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
	// Ќормал≥зуЇмо вектор, щоб швидк≥сть була стаб≥льною
	ProjectileMovement->Velocity = ShootDirection.GetSafeNormal() * ProjectileMovement->InitialSpeed;
}

void ABaseProjectile2D::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Ћог, щоб точно побачити удар
	UE_LOG(LogTemp, Error, TEXT("BAM! Hit Actor: %s"), *OtherActor->GetName());

	if ((IsValid(OtherActor)) && (OtherActor != this))
	{
		//Destroy(); // ѕоки закоментовано дл€ тесту
	}
}