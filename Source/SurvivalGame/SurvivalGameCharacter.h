// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BFI_Interactive.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SurvivalGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FInventoryStruct;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASurvivalGameCharacter : public ACharacter, public IBFI_Interactive
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* EquippedItemMesh;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Sprinting Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerWidget> WidgetToDisplay;
	
	/** Stat Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStatComponent* StatComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
public:
	ASurvivalGameCharacter();
	
	void PlayerHasDied();
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void EndMove(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called or sprint actions */
	void StartSprint(const FInputActionValue& Value);
	void EndSprint(const FInputActionValue& Value);

	/** Inventory action */
	void ToggleInventory(const FInputActionValue& Value);

	/** Interact Action */
	void TryToInteract(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void StaminaHasRunOut();

	void SetEquippedItemMesh(FInventoryStruct* InventoryStructIn);
	UInventoryComponent* GetInventoryComp() const { return InventoryComponent; }

	void DealWithNewItem(const FString ItemName, UTexture2D* ItemIcon, const int32 ItemQuantity);
private:
	UPROPERTY()
	UPlayerWidget* PlayerWidgetRef;

	UPROPERTY()
	APlayerController* PlayerController;
	
	bool bIsMoving;
	bool bIsSprinting;
	bool bOutOfStamina;
	float DefaultWalkSpeed;
	float SprintingSpeed;

	void Interact();
};

