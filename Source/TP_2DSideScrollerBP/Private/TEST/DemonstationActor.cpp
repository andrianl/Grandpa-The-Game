// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST/DemonstationActor.h"

// Sets default values
ADemonstationActor::ADemonstationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	Sphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADemonstationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADemonstationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADemonstationActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OVERLAP"));
	}
}

