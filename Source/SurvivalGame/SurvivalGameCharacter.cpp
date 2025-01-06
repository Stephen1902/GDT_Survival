// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameCharacter.h"

#include "BuildingComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "PlayerWidget.h"
#include "StatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASurvivalGameCharacter

ASurvivalGameCharacter::ASurvivalGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Scene capture camera for viewing the player in the inventory
	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Comp"));
	SceneCaptureComp->SetupAttachment(GetRootComponent());
	SceneCaptureComp->SetRelativeLocation(FVector(325.f, 0.f, 0.f));
	SceneCaptureComp->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
	SceneCaptureComp->FOVAngle = 20.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "head");
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the mesh component which will display equipped items
	EquippedItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Equipped Items"));
	EquippedItemMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "EquippedItem_r");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Stat Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	BuildingComponent = CreateDefaultSubobject<UBuildingComponent>(TEXT("Building Component"));

	bIsMoving = false;
	bIsSprinting = false;
	bOutOfStamina = false;
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintingSpeed = 600.f;
	
}

void ASurvivalGameCharacter::PlayerHasDied()
{
	CameraBoom->TargetArmLength = 400.f;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void ASurvivalGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	if (WidgetToDisplay)
	{
		PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerWidgetRef = CreateWidget<UPlayerWidget>(PlayerController, WidgetToDisplay);
			if (PlayerWidgetRef)
			{
				PlayerWidgetRef->AddToViewport();
				if (StatComponent)
				{
					StatComponent->SetPlayerWidgetRef(PlayerWidgetRef);
				}
			}
		}
	}

	if (InventoryComponent)
	{
		InventoryComponent->SetPlayerCharacterRef(this);
	}

	if (BuildingComponent)
	{
		BuildingComponent->SetPlayerCharacterRef(this);
	}
	if (AttackMontage)
	{
		GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASurvivalGameCharacter::AnimMontageEnded);
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ASurvivalGameCharacter::OnAnimNotify);
	}
	
	DefaultWalkSpeed =  GetCharacterMovement()->GetMaxSpeed();
	if (DefaultWalkSpeed > SprintingSpeed)
	{
		SprintingSpeed = DefaultWalkSpeed * 1.5f;
	}

	// Make sure that only the character is rendered, not the background as well
	SceneCaptureComp->ShowOnlyActorComponents(this, true);

	OnTakeAnyDamage.AddDynamic(this, &ASurvivalGameCharacter::OnDamageReceived);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivalGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get a PlayerController in SetupPlayerInputComponent"));
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalGameCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASurvivalGameCharacter::EndMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalGameCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASurvivalGameCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASurvivalGameCharacter::EndSprint);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ASurvivalGameCharacter::ToggleInventory);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASurvivalGameCharacter::TryToInteract);
		EnhancedInputComponent->BindAction(LeftStrikeAction, ETriggerEvent::Started, this, &ASurvivalGameCharacter::LeftStrike);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvivalGameCharacter::Move(const FInputActionValue& Value)
{
	if (Value.IsNonZero())
	{
		bIsMoving = true;

		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ASurvivalGameCharacter::EndMove(const FInputActionValue& Value)
{
	bIsMoving = false;
}

void ASurvivalGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASurvivalGameCharacter::StartSprint(const FInputActionValue& Value)
{
	if (bIsMoving)
	{
		if (!bIsSprinting && !bOutOfStamina)
		{
			bIsSprinting = true;
			StatComponent->SetIsRegainingStamina(false);
			GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		}
	}
	// If the player is not moving but is holding down the Sprint action, they're not using stamina
	else if (bIsSprinting)
	{
		StatComponent->SetIsRegainingStamina(true);
		bIsSprinting = false;
	}
}

void ASurvivalGameCharacter::EndSprint(const FInputActionValue& Value)
{
	StaminaHasRunOut();
	bOutOfStamina = false;
}

void ASurvivalGameCharacter::ToggleInventory(const FInputActionValue& Value)
{
	if (InventoryComponent)
	{
		InventoryComponent->DealWithInventoryButtonPress(PlayerController);
	}
}

void ASurvivalGameCharacter::StaminaHasRunOut()
{
	bOutOfStamina = true;
	StatComponent->SetIsRegainingStamina(true);
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ASurvivalGameCharacter::SetEquippedItemMesh(FInventoryStruct* InventoryStructIn)
{
	EquippedItemMesh->SetStaticMesh(InventoryStructIn->DisplayMesh);
	if (InventoryComponent)
	{
		InventoryComponent->SetEquippedItem(InventoryStructIn);
	}

	if (PlayerWidgetRef)
	{
		PlayerWidgetRef->UpdateEquippedItem(InventoryStructIn);
	}
	
	// Automatically close the inventory
	ToggleInventory(0);
	
}

void ASurvivalGameCharacter::DealWithNewItem(const FString ItemName, UTexture2D* ItemIcon, const int32 ItemQuantity)
{
	if (PlayerWidgetRef)
	{
		PlayerWidgetRef->SetNewItemInfo(ItemName, ItemIcon, ItemQuantity);
		PlayerWidgetRef->PlayNewItemAnim();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerWidgetRef is not valid."));
	}
}

void ASurvivalGameCharacter::TryToInteract(const FInputActionValue& Value)
{
	InteractiveResource(0.f);
}

void ASurvivalGameCharacter::LeftStrike(const FInputActionValue& Value)
{
	if (!BuildingComponent->GetIsInBuildMode())
	{
		if (AttackMontage)
		{
			if (bCanAttack)
			{
				bCanAttack = false;
				GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
			}
		}
		else
		{
			InteractiveResource(1.0f);
		}
	}
	else
	{
		BuildingComponent->PlaceItemInWorld();
	}
}

void ASurvivalGameCharacter::InteractiveResource(const float HitDamage)
{
	if (PlayerController)
	{
		FHitResult HitResult;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace;
		ObjectsToTrace.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		ObjectsToTrace.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		TArray<AActor*> IgnoredActors;

		FRotator ActorRot = GetActorRotation();
		if (const APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager)
		{
			ActorRot = PlayerCameraManager->GetCameraRotation();
		}
		
		const FVector StartLoc = GetActorLocation();
		const FVector EndLoc = (UKismetMathLibrary::GetForwardVector(ActorRot) * InteractionDistance) + StartLoc; 
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoc, EndLoc, 56.f, ObjectsToTrace, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, true);
		if (HitResult.GetActor())
		{
			FInventoryStruct ItemToGet;
			
			if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UBFI_Interactive::StaticClass()))
			{
				Execute_OnInteract(HitResult.GetActor(), HitDamage, ItemToGet, PlayerController, this);
			}
			else if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetComponent(), UBFI_Interactive::StaticClass()))
			{
				Execute_OnInteractFoliage(HitResult.GetComponent(), HitResult.Item, HitDamage, ItemToGet);

				// Set the hit location of the HitResult to use on the animation
				HitLocation = HitResult.Location;
				HitLocation.Z += 50.f;
			}
			else
			{
				// Assume it is an animal of some sort that can be damaged
				const FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(10.f, DamageEvent, GetController(), this);
				if (ACharacter* Character = Cast<ACharacter>(HitResult.GetActor()))
				{
					FVector AmountToLaunch = GetActorForwardVector() * 500.f;
					AmountToLaunch.Z += 50.f;
					Character->LaunchCharacter(AmountToLaunch, false, false);
				}
			}

			if (!ItemToGet.ItemName.IsEmpty())
			{
				InventoryComponent->AddItem(&ItemToGet);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to hit an actor with the line trace"));
		}
	}
}

void ASurvivalGameCharacter::AnimMontageEnded(UAnimMontage* AnimMontage, bool WasInterrupted/*bInterrupted*/)
{
	bCanAttack = true;
}

void ASurvivalGameCharacter::OnAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	InteractiveResource(1.0f);

	if (InteractHitNiagara)
	{
		/*UNiagaraComponent* NiagaraComp = */UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InteractHitNiagara,  HitLocation, FRotator(180.f, 0.f, 0.f));
	}
}

void ASurvivalGameCharacter::OnDamageReceived(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("%f damaged received"), Damage);
	if (StatComponent)
	{
		StatComponent->CauseDamage(Damage);
	}
}
