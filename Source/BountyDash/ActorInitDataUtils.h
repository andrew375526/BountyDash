#pragma once

#include "Containers/Array.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AMyTargetPoint;
class UStaticMesh;
class AFloor;

namespace ActorInitDataUtils
{
	auto GetTargetPoints(const class UWorld& World)
		-> const TArray<TWeakObjectPtr<AMyTargetPoint>>&;

	double GetMeshXBounds(const UStaticMesh&);
	double GetXScenePositionFromOrigin(const UStaticMesh&, int NumOfMeshes = 1);

	class Position
	{
	public:
		static double GetKillPoint();
		static double GetSpawnPoint();
		static bool SetKillPoint(const AFloor&);
		static bool SetSpawnPoint(const AFloor&);

	private:
		static inline double KillPoint{};
		static inline double SpawnPoint{};
	};
}
