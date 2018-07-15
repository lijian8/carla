// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"
#include "Carla/Actor/ActorRegistry.h"

#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Templates/Function.h"

#include "ActorDispatcher.generated.h"

/// Actor in charge of spawning all the Carla actors.

/// Note that this class itself does not spawn actors,
UCLASS()
class CARLA_API AActorDispatcher : public AActor
{
  GENERATED_BODY()

public:

  using SpawnFunctionType = TFunction<AActor*(const FTransform &, const FActorDescription &)>;

  /// Bind a definition to a spawn function. When SpawnActor is called with a
  /// matching description @a Functor is called.
  ///
  /// @warning Invalid definitions are ignored.
  void Bind(FActorDefinition Definition, SpawnFunctionType SpawnFunction);

  /// Spawns an actor based on @a ActorDescription at @a Transform. To properly
  /// despawn an actor created with this function call DestroyActor.
  ///
  /// Return nullptr on failure.
  UFUNCTION(BlueprintCallable)
  AActor *SpawnActor(
      const FTransform &Transform,
      const FActorDescription &ActorDescription);

  /// Kills an actor, properly removing it from the registry.
  UFUNCTION(BlueprintCallable)
  void DestroyActor(AActor *Actor);

  UFUNCTION(BlueprintCallable)
  const TArray<FActorDefinition> &GetActorDefinitions() const
  {
    return Definitions;
  }

  const UActorRegistry &GetActorRegistry() const
  {
    return Registry;
  }

private:

  TArray<FActorDefinition> Definitions;

  TArray<SpawnFunctionType> SpawnFunctions;

  UActorRegistry Registry;
};
