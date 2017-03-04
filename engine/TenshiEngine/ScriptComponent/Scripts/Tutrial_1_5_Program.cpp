#include "Tutrial_1_5_Program.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "OutputGimic.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tutrial_1_5_Program::Initialize(){
	m_ComboClear = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tutrial_1_5_Program::Start(){
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->ClearCombo();
}

//���t���[���Ă΂�܂�
void Tutrial_1_5_Program::Update(){
	if (UniqueObject::GetPlayer()->GetScript<PlayerController>()->GetHitComboCount() > 20.0f && !m_ComboClear) {
		auto sender = OutputGimic::GetOutputGimic(m_HintDraw);
		if (sender)sender->OnStart(gameObject);
		m_ComboClear = true;
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Tutrial_1_5_Program::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Tutrial_1_5_Program::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Tutrial_1_5_Program::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Tutrial_1_5_Program::OnCollideExit(GameObject target){
	(void)target;
}