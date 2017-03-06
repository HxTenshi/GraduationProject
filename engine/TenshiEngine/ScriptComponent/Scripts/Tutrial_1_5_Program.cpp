#include "Tutrial_1_5_Program.h"
#include "UniqueObject.h"
#include "PlayerController.h"
#include "OutputGimic.h"
#include "hintDraw.h"
#include "EnemyManager.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tutrial_1_5_Program::Initialize(){
	m_ComboClear = false;
	m_Count = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Tutrial_1_5_Program::Start(){
	UniqueObject::GetPlayer()->GetScript<PlayerController>()->ClearCombo();
	EnemyManager::EnemyAllDelete();
}

//���t���[���Ă΂�܂�
void Tutrial_1_5_Program::Update(){
	if (UniqueObject::GetPlayer()->GetScript<PlayerController>()->GetHitComboCount() >= 20 && !m_ComboClear) {
		m_ComboTextureObject = Hx::Instance(m_ComboTexture);
		m_ComboClear = true;
	}

	if (m_ComboClear) {
		m_Count += Hx::DeltaTime()->GetDeltaTime();
	}

	if (m_Count > m_Time) {
		Hx::DestroyObject(m_ComboTextureObject);
		hintDraw::OnStart_(gameObject);
		gameObject->RemoveComponent<ScriptComponent>();
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