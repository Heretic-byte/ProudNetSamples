// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleHUD.h"

void ASimpleHUD::DrawHUD()
{
	Super::DrawHUD();

	// ����� �׸��ϴ�.
	DrawRect(FLinearColor::FromSRGBColor(FColor(49,77,121)), 0, 0, Canvas->ClipX, Canvas->ClipY);
}
