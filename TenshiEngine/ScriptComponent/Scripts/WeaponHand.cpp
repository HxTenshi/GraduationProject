#include "WeaponHand.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponHand::Start(){

}

//���t���[���Ă΂�܂�
void WeaponHand::Update(){

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
