#include "BountyDashHUD.h"

#include "GameModeUtils.h"
#include "BountyDashCharacter.h"
#include "BountyDashGameMode.h"

#include "Containers/UnrealString.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "Math/MathFwd.h"

ABountyDashHUD::ABountyDashHUD()
{
	ConstructorHelpers::FObjectFinder<UFont> Font{
		TEXT("/Game/Barrel_Hopper/BountyDashFont.BountyDashFont") };

	if (Font.Succeeded()) HUDFont = Font.Object;
}

void ABountyDashHUD::DrawHUD()
{
	Super::DrawHUD();

	const auto* Player{
		Cast<ABountyDashCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)) };

	auto* GameMode{
		GameModeUtils::GetCustomGameMode<ABountyDashGameMode>(*GetWorld()) };

	const FString HUDText{ FString::Format(
		TEXT("Score: {0} Runtime: {1}"),
		{ Player->GetScore(), GameMode->GetRuntime() }) };

	DrawText(HUDText, FColor::Yellow, 50, 50, HUDFont);

	if (GameMode->IsGameOver())
	{
		FVector2f GameOverSize{};
		const FString GameOverText{ TEXT("GAME OVER!!! Press R to Restart!") };

		GetTextSize(
			GameOverText,
			GameOverSize.X,
			GameOverSize.Y,
			HUDFont);

		const FIntVector2 ScreenDimensions{ Canvas->SizeX, Canvas->SizeY };

		DrawText(
			GameOverText,
			FColor::Yellow,
			(ScreenDimensions.X - GameOverSize.X) / 2.0,
			(ScreenDimensions.Y - GameOverSize.Y) / 2.0,
			HUDFont);

		GameMode->PauseGame(true);
	}
}
