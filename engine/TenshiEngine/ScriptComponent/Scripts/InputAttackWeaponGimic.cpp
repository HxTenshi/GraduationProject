#include "InputAttackWeaponGimic.h"
#include "h_standard.h"
#include "OutputGimic.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputAttackWeaponGimic::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputAttackWeaponGimic::Start(){

}

//���t���[���Ă΂�܂�
void InputAttackWeaponGimic::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void InputAttackWeaponGimic::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputAttackWeaponGimic::OnCollideBegin(GameObject target){
	(void)target;
}

#include "Weapon.h"
//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputAttackWeaponGimic::OnCollideEnter(GameObject target){
	if (!target)return;
	if (!m_OutputGimic)return;

	auto gimic = OutputGimic::GetOutputGimic(m_OutputGimic);
	if (!gimic)return;

	if (auto w = target->GetScript<Weapon>()) {
		if (w->isAttack()) {
			gimic->OnStart(gameObject);
			return;
		}
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void InputAttackWeaponGimic::OnCollideExit(GameObject target){
	(void)target;
}