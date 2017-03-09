#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "h_standard.h"

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeEventPlayerStatus::Start(){

	Hx::Debug()->Log("Name : "+gameObject->Name());
	Hx::Debug()->Log("Parent : " + gameObject->mTransform->GetParent()->Name());
}

//���t���[���Ă΂�܂�
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
