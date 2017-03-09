#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "h_standard.h"

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChangeEventPlayerStatus::Start(){

	Hx::Debug()->Log("Name : "+gameObject->Name());
	Hx::Debug()->Log("Parent : " + gameObject->mTransform->GetParent()->Name());
}

//毎フレーム呼ばれます
void ChangeEventPlayerStatus::Update(){
	OnSetSP();
	LockWeapon();
	ThrowWeapon();
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
		if (mDebug)Hx::Debug()->Log("WeaponLock : "+std::to_string(mLock));
		player->SetLockAction(PlayerController::LockAction::DropWeapon,mLock);
	}
}

void ChangeEventPlayerStatus::ThrowWeapon()
{
	auto player = UniqueObject::GetPlayer()->GetScript<PlayerController>();
	if (player) {
		if(mDebug)Hx::Debug()->Log("WeaponThrow : " + std::to_string(mThrow));
		player->SetLockAction(PlayerController::LockAction::ThrowWeapon, mThrow);
	}
}
