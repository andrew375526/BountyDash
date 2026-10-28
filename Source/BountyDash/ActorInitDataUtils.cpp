#include "ActorInitDataUtils.h"
#include "Floor.h"
#include "MyTargetPoint.h"
#include "TargetPointManager.h"
#include "Containers/Array.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/WeakObjectPtrTemplates.h"

namespace ActorInitDataUtils
{
	auto GetTargetPoints(const UWorld& World)
		-> const TArray<TWeakObjectPtr<AMyTargetPoint>>&
	{
		return World.GetSubsystem<UTargetPointManager>()->GetTargetPoints();
	}

	double GetMeshXBounds(const UStaticMesh& SM)
	{
		return SM.GetBounds().BoxExtent.X * 2.0;
	}

	double GetXScenePositionFromOrigin(const UStaticMesh& SM, int NumOfMeshes)
	{
		return GetMeshXBounds(SM) * (NumOfMeshes - 1) / 2.0;
	}

	namespace
	{
		double CalculateKillPoint(const UStaticMesh& SM, int NumOfMeshes)
		{
			return -GetXScenePositionFromOrigin(SM, NumOfMeshes)
				- (GetMeshXBounds(SM) * 0.5);
		}
	}

	double Position::GetKillPoint()
	{
		return KillPoint;
	}

	double Position::GetSpawnPoint()
	{
		return SpawnPoint;
	}

	bool Position::SetKillPoint(const AFloor& F)
	{
		check(F.GetFloorMesh());
		if (!F.GetFloorMesh()) return false;

		KillPoint = CalculateKillPoint(
			*(F.GetFloorMesh()), F.GetNumRepeatingMesh());

		return true;
	}

	bool Position::SetSpawnPoint(const AFloor& F)
	{
		check(F.GetFloorMesh());
		if (!F.GetFloorMesh()) return false;

		SpawnPoint = -CalculateKillPoint(
			*(F.GetFloorMesh()), F.GetNumRepeatingMesh());

		return true;
	}
}
