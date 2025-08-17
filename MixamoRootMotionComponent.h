#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MixamoRootMotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIXAMOANIMATIONS_API UMixamoRootMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMixamoRootMotionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ActivateMotion();
	UFUNCTION(BlueprintCallable)
	void DeactivateMotion(bool SnapBackToStartLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Hip Root Motion")
	FName HipBoneName = "Hips";
	UPROPERTY(VisibleAnywhere, Category = "Hip Root Motion")
	bool bEnableHipMotion = false;
	UPROPERTY(EditAnywhere, Category = "Hip Root Motion")
	bool bDebugDrawHipMovement = false;

	FVector PreviousHipLocation;
	FTransform InitialMeshRelativeTransform;
	class USkeletalMeshComponent* Mesh;
	class ACharacter* OwnerCharacter;
};
