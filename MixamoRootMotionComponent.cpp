#include "MixamoRootMotionComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UMixamoRootMotionComponent::UMixamoRootMotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMixamoRootMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Mesh = OwnerCharacter->FindComponentByClass<USkeletalMeshComponent>();
}

// Called every frame
void UMixamoRootMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bEnableHipMotion || !Mesh || HipBoneName == NAME_None) return;

	FVector CurrentHipLocation = Mesh->GetBoneLocation(HipBoneName, EBoneSpaces::WorldSpace);
	FVector Delta = CurrentHipLocation - PreviousHipLocation;

	// vertical root motion is not supported because it would conflict with CharacterMovementComponent
	Delta.Z = 0.0f;

	if (!Delta.IsNearlyZero())
	{
		if (bDebugDrawHipMovement)
		{
			DrawDebugLine(
				GetWorld(),
				GetOwner()->GetActorLocation(),
				GetOwner()->GetActorLocation() + Delta,
				FColor::Cyan, true);
		}
		FVector LocalDelta = Mesh->GetComponentTransform().InverseTransformVectorNoScale(Delta);
		Mesh->AddLocalOffset(-LocalDelta);

		FHitResult Hit;
		OwnerCharacter->GetCharacterMovement()->SafeMoveUpdatedComponent(Delta, OwnerCharacter->GetActorRotation(), true, Hit);
	}

	PreviousHipLocation = CurrentHipLocation;
}

void UMixamoRootMotionComponent::ActivateMotion()
{
	if (Mesh)
	{
		InitialMeshRelativeTransform = Mesh->GetRelativeTransform();
		PreviousHipLocation = Mesh->GetBoneLocation(HipBoneName, EBoneSpaces::WorldSpace);
	}
	bEnableHipMotion = true;
}

void UMixamoRootMotionComponent::DeactivateMotion(bool SnapBackToStartLocation)
{
	if (!bEnableHipMotion)
		return;

	bEnableHipMotion = false;
	if (SnapBackToStartLocation && Mesh)
	{
		Mesh->SetRelativeTransform(InitialMeshRelativeTransform);
	}
}