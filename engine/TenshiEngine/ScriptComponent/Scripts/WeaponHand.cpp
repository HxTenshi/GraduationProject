#include "WeaponHand.h"
#include "h_standard.h"
#include "Weapon.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Initialize(){
	m_AttackTime = 0.0f;
	m_IsGuard = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Start(){

}

//���t���[���Ă΂�܂�
void WeaponHand::Update(){
	if (!mWeapon)return;



	if (m_IsGuard) {
		if(m_GuardPos)
			mWeapon->mTransform->SetParent(m_GuardPos);
		m_AttackTime = 0.0f;
	}
	else {
		mWeapon->mTransform->SetParent(gameObject);

		if (m_AttackTime > 0.0f) {
			m_AttackTime -= Hx::DeltaTime()->GetDeltaTime();
			m_AttackTime = max(m_AttackTime, 0.0f);

			mWeapon->mTransform->Rotate(XMVectorSet(m_AttackTime * 40.0f, 0, 0, 1));
		}
	}

	m_IsGuard = false;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponHand::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponHand::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponHand::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponHand::SetWeapon(GameObject weapon)
{
	if (mWeapon)return;
	if (mWeapon) {
		Hx::DestroyObject(mWeapon);
	}
	mWeapon = weapon;
	if (!mWeapon)return;
	mWeapon->mTransform->SetParent(gameObject);
	mWeapon->mTransform->Position(XMVectorZero());
	mWeapon->mTransform->Rotate(XMVectorZero());

	if (auto scr = mWeapon->GetScript<Weapon>()) {
		scr->GetWeapon();
	}

}

void WeaponHand::Attack()
{
	m_AttackTime = 1.0f;
}

void WeaponHand::Guard()
{
	m_IsGuard = true;

}

void WeaponHand::ThrowAway()
{
	if (!mWeapon)return;
	if (auto scr = mWeapon->GetScript<Weapon>()) {
		//auto v = gameObject->mTransform->Forward();
		//scr->ThrowAway(v);
		mWeapon = NULL;
		scr->ThrowAway();
	}
}
