#include "InputEnterObjectGimic.h"

#include "OutputGimic.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputEnterObjectGimic::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void InputEnterObjectGimic::Start(){

}

//���t���[���Ă΂�܂�
void InputEnterObjectGimic::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void InputEnterObjectGimic::Finish(){
	
}

#include "h_standard.h"
//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputEnterObjectGimic::OnCollideBegin(GameObject target){

	if (!target)return;
	if (!_m_OutputGimic)return;

	auto gimic = OutputGimic::GetOutputGimic(_m_OutputGimic);
	if (!gimic)return;

	if (m_TargetObj) {
		if (target == m_TargetObj) {

			gimic->OnStart(gameObject);
			return;
		}
	}
	if (!m_TargetName.empty()) {

		if (target->Name() == m_TargetName) {

			gimic->OnStart(gameObject);
			return;
		}
	}
	//if (m_TargetLayer) {
		if (target->GetLayer() == m_TargetLayer) {

			gimic->OnStart(gameObject);
			return;
		}
	//}
	//if (m_TargetScript.empty()) {
	//	if (target->GetScript<>() == m_TargetScript) {

	//		return;
	//	}
	//}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void InputEnterObjectGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void InputEnterObjectGimic::OnCollideExit(GameObject target){
	(void)target;
}