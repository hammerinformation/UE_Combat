// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

#include "ZombieAi.generated.h"
UENUM()

enum class State : uint8 {
	RandomMovement = 0,
	Attack = 1,
	IdleWalkRun = 2,
	Die = 3,
	Stop = 4
};
UCLASS()
class COMBAT_API AZombieAi : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieAi();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


	UFUNCTION()
		void OnTriggerExit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private :
	bool bPlayerSeen = false;

public:
	virtual void Tick(float DeltaTime) override;
	
	class AAIController* Ai;
	class UNavigationSystemV1* NavigationSystem;
	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditAnywhere)
		State CurrentState;
	void SetState(State State);
	State GetState();
	void RandomMovement();
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AttackAnimMontage;

	void Die();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);


		UFUNCTION()
		void OnSeePawn(APawn* Pawn);

	void Attack();
	void MoveToTargetActor();
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* Trigger;
	UFUNCTION()
		void OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
