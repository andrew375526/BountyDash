#pragma once

namespace GameModeUtils
{
	template <typename T>
	T* GetCustomGameMode(const class UWorld& WorldContext)
	{
		return Cast<T>(WorldContext.GetAuthGameMode());
	}
}
