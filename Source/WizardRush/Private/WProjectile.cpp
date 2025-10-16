// Fill out your copyright notice in the Description page of Project Settings.


#include "WProjectile.h"
#include "Components/SphereComponent.h"
//#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "WizPMC.h"

// Sets default values
AWProjectile::AWProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("SpereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	// Legacy For Gideon Cascade Projectile
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("CascadeEffectComp");
	EffectComp->SetupAttachment(SphereComp);

	
	// New System for VFX Pack
	NiagaraEffectComp = CreateDefaultSubobject<UNiagaraComponent>("NiagaraEffectComp");
	NiagaraEffectComp->SetupAttachment(SphereComp);

	
	MovementComp = CreateDefaultSubobject<UWizPMC>("MovementComp");
	MovementComp->InitialSpeed = ProjectileSpeed;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}

// Called when the game starts or when spawned
void AWProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

