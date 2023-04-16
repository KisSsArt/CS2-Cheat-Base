#include "BasePlayerController.h"

#include "CSPlayerPawnBase.h"

CSPlayerPawnBase* BasePlayerController::getPawn()
{
	return pawn().get<CSPlayerPawnBase>();
}
