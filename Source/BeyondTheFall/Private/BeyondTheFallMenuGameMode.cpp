#include "BeyondTheFallMenuGameMode.h"

#include "BeyondTheFallPlayerControl.h"

ABeyondTheFallMenuGameMode::ABeyondTheFallMenuGameMode()
{
	PlayerControllerClass = ABeyondTheFallPlayerControl::StaticClass();
}