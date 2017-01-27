#include "ChangePlayerState.h"
# include "PlayerController.h"
#include "h_standard.h"
bool ChangePlayerState::OnStart(GameObject Sender)
{
	Hx::Debug()->Log("StateChange");
	PlayerController* ctr = Hx::FindActor("Player")->GetScript<PlayerController>();
	if (ctr)
	{
		if (m_IsLock) {
			Hx::Debug()->Log("�X�e�[�g : Lock");
			ctr->SetPlayerState(PlayerController::PlayerState::Movie);
		}
		else {
			Hx::Debug()->Log("�X�e�[�g : Free");
			ctr->SetPlayerState(PlayerController::PlayerState::Free);
		}
		return true;
	}
	return false;
}
