

#include "ZombieAi.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AZombieAi::AZombieAi()
{
	PrimaryActorTick.bCanEverTick = true;
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 3500.0f;
	PawnSensing->SetPeripheralVisionAngle(60);
	PawnSensing->SensingInterval = 0.2f;
	PawnSensing->bOnlySensePlayers = false;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
}


void AZombieAi::BeginPlay()
{
	Super::BeginPlay();
	Ai = Cast< AAIController>(GetController());
	NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	Ai->ReceiveMoveCompleted.AddDynamic(this, &AZombieAi::OnMoveCompleted);

	
	PawnSensing->OnSeePawn.AddDynamic(this, &AZombieAi::OnSeePawn);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AZombieAi::OnHit);
	
	bPlayerSeen = false;
	

	FTimerHandle TH_FirstState;
	GetWorld()->GetTimerManager().SetTimer(TH_FirstState, this, &AZombieAi::RandomMovement, FMath::RandRange(0.1f, 0.3f), false);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 40.0f, 0.0f);

}

void AZombieAi::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AZombieAi::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AZombieAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombieAi::SetState(State State)
{
	CurrentState = State;
	switch (CurrentState)
	{
	case State::RandomMovement:
		RandomMovement();
		break;
	case State::Attack:
		Attack();
		break;
	case State::IdleWalkRun:
		break;
	case State::Die:
		Die();
		break;
	case State::Stop:
		break;
	default:
		break;
	}
}

State AZombieAi::GetState()
{
	return CurrentState;

}

void AZombieAi::RandomMovement()
{
	FNavLocation NavLocation;
	NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), 10000.0f, NavLocation);
	Ai->MoveToLocation(NavLocation.Location);


}

void AZombieAi::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) {


	if (GetState() == State::RandomMovement && Result == EPathFollowingResult::Success) {
		RandomMovement();
	}
	if (GetState() == State::Attack && Result == EPathFollowingResult::Success) {
		Attack();
	}
}

void AZombieAi::Die() {

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AZombieAi::OnSeePawn(APawn* Pawn) {
	if (Pawn&& Pawn->GetName()=="Player"&& bPlayerSeen==false) {
		TargetActor = Pawn;
		MoveToTargetActor();
	}
}

void AZombieAi::Attack()
{

	if (GetMesh()->GetAnimInstance()&& AttackAnimMontage) {
		GetMesh()->GetAnimInstance()->Montage_Play(AttackAnimMontage);
	}
}

void AZombieAi::MoveToTargetActor()
{
	//Ai->StopMovement();

	if (TargetActor) {
		bPlayerSeen = true;

		Ai->MoveToActor(TargetActor,1.0f);
}
}

void AZombieAi::OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor->GetName() == "Sword") {
		Die();
	}
}
