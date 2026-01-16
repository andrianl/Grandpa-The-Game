#include "Actors/BaseProjectile2D.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile2D::ABaseProjectile2D()
{
    // Performance: Projectiles rarely need to tick. Movement is handled by the Component.
    PrimaryActorTick.bCanEverTick = false;

    // 1. Setup Collision
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(10.0f);

    // Using a standard profile. Ensure your Project Settings have this configured.
    CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComp->SetNotifyRigidBodyCollision(true);
    RootComponent = CollisionComp;

    // 2. Setup Visuals
    SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
    SpriteComp->SetupAttachment(RootComponent);
    SpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 3. Setup Movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = RootComponent;
    ProjectileMovement->InitialSpeed = 800.0f;
    ProjectileMovement->MaxSpeed = 1000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // Linear flight for 2D

    // Constrain movement to the XZ plane (Standard for most 2D side-scrollers)
    ProjectileMovement->bConstrainToPlane = true;
    ProjectileMovement->SetPlaneConstraintNormal(FVector(0.0f, 1.0f, 0.0f));

    // 4. Memory Safety: Automatically destroy after 3 seconds if nothing is hit
    InitialLifeSpan = 3.0f;
}

void ABaseProjectile2D::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(CollisionComp))
    {
        CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile2D::OnHit);
    }
}

void ABaseProjectile2D::FireInDirection(const FVector& ShootDirection)
{
    if (IsValid(ProjectileMovement))
    {
        ProjectileMovement->Velocity = ShootDirection.GetSafeNormal() * ProjectileMovement->InitialSpeed;
    }
}

void ABaseProjectile2D::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Ignore hits with the actor who fired the projectile
    if (!IsValid(OtherActor) || OtherActor == this || OtherActor == GetOwner())
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Projectile Impact: %s hit %s"), *GetName(), *OtherActor->GetName());

    // Logic for applying damage goes here (e.g., using Gameplay Effects or ApplyDamage)

    // Destroy the projectile on impact
    Destroy();
}