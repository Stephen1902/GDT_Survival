// Copyright 2024 DME Games

#include "PlayerWidget.h"

#include "Kismet/GameplayStatics.h"

void UPlayerWidget::PlayDamageAnim()
{
	if (DamageAnimation && !IsAnimationPlaying(DamageAnimation))
	{
		PlayAnimation(DamageAnimation);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerDamageSound, GetOwningPlayer()->GetPawn()->GetActorLocation());
	}
}
