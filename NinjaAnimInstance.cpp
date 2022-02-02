// Fill out your copyright notice in the Description page of Project Settings.


#include "NinjaAnimInstance.h"

void UNinjaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (TryGetPawnOwner()) {
		Speed = TryGetPawnOwner()->GetVelocity().Size();




	}
}