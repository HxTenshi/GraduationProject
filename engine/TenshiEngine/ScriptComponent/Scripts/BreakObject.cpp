#include "BreakObject.h"
#include "AttackFilter.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BreakObject::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BreakObject::Start(){

}

//���t���[���Ă΂�܂�
void BreakObject::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void BreakObject::Finish(){

}
#include"Weapon.h"
//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BreakObject::OnCollideBegin(GameObject target){
	if (!target)return;
	auto fil = AttackFilter::Filter::eAll;
	if (m_OnlyBombBreak) {
		fil = AttackFilter::Filter::eBomb;
	}
	if (AttackFilter::IsAttack(target,fil)) {
		if (m_NormalObject) {
			m_NormalObject->Disable();
		}
		if (m_BreakObject) {
			m_BreakObject->Enable();
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BreakObject::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void BreakObject::OnCollideExit(GameObject target){
	(void)target;
}