// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"

#define WIN32_LEAN_AND_MEAN
#include "ProudNetClient.h"
#include "ProudNetActor.h"

#include "SimpleUE4GameModeBase.generated.h"

UCLASS()
class SIMPLEUE4_API ASimpleUE4GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// �޴� ������ �ٲٴ� �Լ��Դϴ�. �������Ʈ�� ���ؼ� ���Ǿ����ϴ�.
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	// Send ��ư�� ������ �� �������Ʈ�� ���ؼ� ȣ��Ǿ����� �������� �޼��� �۽� �Լ��Դϴ�.
	UFUNCTION(BlueprintCallable, Category = "ProudNet")
		void SendRMI();

	// SendP2P ��ư�� ������ �� �������Ʈ�� ���ؼ� ȣ��Ǿ����� P2P �׷� �� �Ǿ����� �޼��� �۽� �Լ��Դϴ�.
	UFUNCTION(BlueprintCallable, Category = "ProudNet")
		void SendP2PRMI();

	// ���� �޼��� ��ü�� �������Ʈ�� ������ �Լ��Դϴ�.
	UFUNCTION(BlueprintCallable, Category = "ProudNet")
		FString GetMessage();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> m_startingWidgetClass;

	UPROPERTY()
		UUserWidget* m_currentWidget;
};
