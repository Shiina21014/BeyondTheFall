#include "Framework/BeyondTFRaceGameState.h"

#include "Net/UnrealNetwork.h"

ABeyondTFRaceGameState::ABeyondTFRaceGameState()
{
	bReplicates = true;
}

void ABeyondTFRaceGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABeyondTFRaceGameState, RacePhase);
	DOREPLIFETIME(ABeyondTFRaceGameState, RaceTime);
	DOREPLIFETIME(ABeyondTFRaceGameState, CountdownNumber);
}