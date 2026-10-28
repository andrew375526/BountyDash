#include "CoinManager.h"

#include "Coin.h"

void UCoinManager::AddCoin(ACoin* Coin)
{
	Coins.Add(Coin);
}

const TArray<TWeakObjectPtr<ACoin>>& UCoinManager::GetCoins() const
{
	return Coins;
}
