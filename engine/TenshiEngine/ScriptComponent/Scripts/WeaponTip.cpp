#include "WeaponTip.h"
#include "Weapon.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTip::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTip::Start(){

}

//���t���[���Ă΂�܂�
void WeaponTip::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponTip::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponTip::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponTip::OnCollideEnter(GameObject target){
	(void)target;
	if (target->GetLayer() == 4) {
		if (m_weapon->GetScript<Weapon>()) {
			m_weapon->GetScript<Weapon>()->WeaponUsePhysX();
		}
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponTip::OnCollideExit(GameObject target){
	(void)target;
}