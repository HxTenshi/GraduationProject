#include "ObjectGenerator.h"

#include"h_standard.h"
#include "OutputGimic.h"

#include <random>


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ObjectGenerator::Initialize(){
	m_GeneratObject = NULL;
	m_Timer = m_Time;
	m_Count = 0;
	m_GeneratFlag = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ObjectGenerator::Start(){

}

//���t���[���Ă΂�܂�
void ObjectGenerator::Update(){
	if (!m_Object.IsLoad())return;
	if (m_UniqueGeneratMode) {
		if (m_GeneratObject)return;
		//Hx::Debug()->Log("Cheack m_GeneratFlag : " + std::to_string(m_GeneratFlag));
		if (m_GeneratFlag) {	
			if (auto gimick = OutputGimic::GetOutputGimic(m_DestroyOutputGimick)) {
				gimick->OnStart(gameObject);
			}
			m_GeneratFlag = false;
		}
	}
	if (m_Num != 0 && m_Count >= m_Num) {
		return;
	}

	m_Timer -= Hx::DeltaTime()->GetDeltaTime();
	m_Timer = max(m_Timer, 0.0f);
	if (m_Timer != 0.0f)return;
	m_Timer = m_Time;
	m_Count++;

	if (m_GeneratPercent!=0.0f) {
		static std::random_device rnd;     // �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
		static std::mt19937 mt(rnd()); //  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h

		float r = mt() / 100000.0f;

		float res = fmod(r, 1.0f);
		res = abs(res);
		if (m_GeneratPercent <= res) {
			return;
		}
	}

	GameObject obj = Hx::Instance(m_Object);
	if (!obj)return;
	if(!m_PositionCopyNot)
		obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
	if(m_RotateCopy)
		obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());

	if (m_UniqueGeneratMode) {
		m_GeneratObject = obj;
		m_GeneratFlag = true;
	}
	if (auto gimick = OutputGimic::GetOutputGimic(m_OutputGimick)) {
		gimick->OnStart(gameObject);
	}

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ObjectGenerator::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ObjectGenerator::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ObjectGenerator::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ObjectGenerator::OnCollideExit(GameObject target){
	(void)target;
}