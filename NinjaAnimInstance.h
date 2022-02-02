
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NinjaAnimInstance.generated.h"


UCLASS()
class COMBAT_API UNinjaAnimInstance : public UAnimInstance
{
public:
	GENERATED_BODY()
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
		float Speed;
};
