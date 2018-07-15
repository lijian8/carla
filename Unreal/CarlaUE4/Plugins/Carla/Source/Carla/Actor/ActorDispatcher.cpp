// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Carla.h"
#include "Carla/Actor/ActorDispatcher.h"

#include "Carla/Actor/ActorBlueprintFunctionLibrary.h"

void AActorDispatcher::Bind(FActorDefinition Definition, SpawnFunctionType Functor)
{
  if (UActorBlueprintFunctionLibrary::CheckActorDefinition(Definition))
  {
    Definition.UId = static_cast<uint32>(SpawnFunctions.Num()) + 1u;
    Definitions.Emplace(Definition);
    SpawnFunctions.Emplace(Functor);
  }
  else
  {
    UE_LOG(LogCarla, Warning, TEXT("Invalid definition ignored"));
  }
}

// const TArray<FActorDefinition> &AActorDispatcher::RetrieveDefinitions()
// {
//   if (!bBlueprintDefinitionsCollected)
//   {
//     for (auto &Definition : GenerateDefinitions())
//     {
//       Bind(Definition, [this](const FTransform &Transform, const FActorDescription &Description) {
//         AActor *Actor = nullptr;
//         SpawnActor(Transform, Description, Actor);
//         return Actor;
//       });
//     }
//     bBlueprintDefinitionsCollected = true;
//   }
//   check(Definitions.Num() == SpawnFunctions.Num());
//   return Definitions;
// }

AActor *AActorDispatcher::SpawnActor(
    const FTransform &Transform,
    const FActorDescription &Description)
{
  if ((Description.UId == 0) || (Description.UId > SpawnFunctions.Num()))
  {
    UE_LOG(LogCarla, Error, TEXT("Invalid ActorDescription \"%s\" (UId=%d)"), *Description.Id, Description.UId);
    return nullptr;
  }
  auto *Actor = SpawnFunctions[Description.UId](Transform, Description);
  if (Actor != nullptr)
  {
    Registry.Register(*Actor);
  }
  return Actor;
}

void AActorDispatcher::DestroyActor(AActor *Actor)
{
  if (Actor != nullptr)
  {
    /// @todo Deregister.
    Actor->Destroy();
  }
}
