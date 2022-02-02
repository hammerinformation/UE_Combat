

#include "Ninja.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "ZombieAi.h"

ANinja::ANinja()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 440.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MaxFlySpeed = 3500.0f;
	GetCharacterMovement()->MaxAcceleration = 200.0f;
	GetCharacterMovement()->BrakingDecelerationFlying = 1500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.0f;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHand"));
}


void ANinja::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ANinja::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ANinja::OnTriggerExit);
}

void ANinja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANinja::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANinja::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ANinja::Attack);

	//PlayerInputComponent->BindAction("StartSprint", IE_Pressed, this, &ANinja::StartSprint);
	//PlayerInputComponent->BindAction("StopSprint", IE_Released, this, &ANinja::StopSprint);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANinja::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANinja::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANinja::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANinja::LookUpAtRate);
}
void ANinja::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Zombie")) {
		//GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity * 0.5f;

		TargetAi = OtherActor;
		bEnemyTriggered = true;
	}
}

void ANinja::OnTriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Zombie")) {

		TargetAi = nullptr;
		bEnemyTriggered = false;

	}
}

void ANinja::MoveForward(float Value)
{
	if (!bMovement) {

		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANinja::MoveRight(float Value)
{
	if (!bMovement) {

		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void ANinja::TurnAtRate(float Rate)
{

	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANinja::LookUpAtRate(float Rate)
{


	AddMovementInput(GetActorUpVector(), Rate * -5);


	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void ANinja::Jump()
{
	ACharacter::Jump();
	if (GetMesh()->GetAnimInstance() && JumpAnimation) {
		//GetMesh()->GetAnimInstance()->Montage_Play(Animations[Index]);
		GetMesh()->GetAnimInstance()->Montage_Play(JumpAnimation);
		bMovement = false;

		GetWorld()->GetTimerManager().SetTimer(TH_Movement, this, &ANinja::EnableMovement, JumpAnimation->GetPlayLength(), false);
	}

	

}

void ANinja::Attack()
{
	int Index = FMath::RandRange(0, Animations.Num() - 1);
	//GetCharacterMovement()->Velocity = FVector::ZeroVector;
	if (GetMesh()->GetAnimInstance() && Animations[Index]) {
		//GetMesh()->GetAnimInstance()->Montage_Play(Animations[Index]);
		GetMesh()->GetAnimInstance()->Montage_Play(Animations[1], 1.3f);
	}
	bMovement = false;

	GetWorld()->GetTimerManager().SetTimer(TH_Movement, this, &ANinja::EnableMovement, Animations[1]->GetPlayLength(), false);


	if (TargetAi && bEnemyTriggered) {
		AZombieAi* Zombie = Cast<AZombieAi>(TargetAi);
		if (Zombie) {





			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ANinja::Kill, Zombie);
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.95f, false);
		}
	}
}

void ANinja::Kill(AZombieAi* Zombie)
{
	Zombie->Die();


}

void ANinja::EnableMovement()
{

	bMovement = true;
	GetWorldTimerManager().ClearTimer(TH_Movement);
}

