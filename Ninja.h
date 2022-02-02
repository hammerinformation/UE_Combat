// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ninja.generated.h"
class AZombieAi;
UCLASS()
class COMBAT_API ANinja : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANinja();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;


private :
	void Jump();
	void Attack();
	void Kill(AZombieAi* Zombie);
	void EnableMovement();
	FTimerHandle TH_Movement;
	bool bMovement = true;
	AActor* TargetAi;
	bool bEnemyTriggered=false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	UFUNCTION()
		void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);


		UFUNCTION()
	void OnTriggerExit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> Animations;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UPROPERTY(EditAnywhere)
		class UBoxComponent* Trigger;
	UPROPERTY(EditAnywhere)

		class UStaticMeshComponent* Sword;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* JumpAnimation;
	
};
