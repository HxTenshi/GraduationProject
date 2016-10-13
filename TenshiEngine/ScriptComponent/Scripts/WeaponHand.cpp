#include "WeaponHand.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Initialize(){
	m_AtackTime = 0.0f;
	m_IsGuard = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Start(){

}

//���t���[���Ă΂�܂�
void WeaponHand::Update(){

	if (m_AtackTime != 0.0f) {
		m_AtackTime -= Hx::DeltaTime()->GetDeltaTime();

		mWeapon->mTransform->Rotate(XMVectorSet(m_AtackTime,0,0,1));
	}

	if (m_IsGuard) {
		if(m_GuardPos)
			mWeapon->mTransform->SetParent(m_GuardPos);
	}
	else {
		mWeapon->mTransform->SetParent(gameObject);
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
	if (mWeapon) {
		Hx::DestroyObject(mWeapon);
	}
	mWeapon = weapon;
	mWeapon->mTransform->SetParent(gameObject);
	mWeapon->mTransform->Position(XMVectorZero());
	mWeapon->mTransform->Rotate(XMVectorZero());
	mWeapon->RemoveComponent<PhysXComponent>();
}

void WeaponHand::Atack()
{
	m_AtackTime = 1.0f;
}

void WeaponHand::Guard()
{
	m_IsGuard = true;

}
