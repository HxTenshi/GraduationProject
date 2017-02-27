#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeEventPlayerStatus::Start(){

}

//���t���[���Ă΂�܂�
void ChangeEventPlayerStatus::Update(){
	OnSetSP();
	LockWeapon();
}

void ChangeEventPlayerStatus::OnSetSP()
{
	if (!mSPUpdate)return;
	auto player = UniqueObject::GetPlayer()->GetScript<PlayerController>();
	if (player) {
		player->SetSpecial(mSP);
	}
}

void ChangeEventPlayerStatus::LockWeapon()
{
	auto player = UniqueObject::GetPlayer()->GetScript<PlayerController>();
	if (player) {
		player->SetLockAction(PlayerController::LockAction::DropWeapon,mLock);
	}
}
