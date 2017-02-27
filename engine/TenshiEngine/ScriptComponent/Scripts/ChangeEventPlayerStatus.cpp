#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChangeEventPlayerStatus::Start(){

}

//毎フレーム呼ばれます
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
