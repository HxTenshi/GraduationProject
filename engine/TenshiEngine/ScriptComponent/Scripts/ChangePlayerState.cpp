#include "ChangePlayerState.h"
# include "PlayerController.h"
bool ChangePlayerState::OnStart(GameObject Sender)
{
	PlayerController* ctr = Hx::FindActor("Player")->GetScript<PlayerController>();
	if (ctr)
	{
		if(!m_IsLock)ctr->SetPlayerState(PlayerController::PlayerState::Lock);
		else ctr->SetPlayerState(PlayerController::PlayerState::Free);
		return true;
	}
	return false;
}
