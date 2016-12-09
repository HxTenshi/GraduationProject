#include "BreakForce.h"

#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BreakForce::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BreakForce::Start(){

	GameObject obj;

	obj = m_Object1;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force,ForceMode::eIMPULSE);
		}
	}
	obj = m_Object2;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object3;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object4;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object5;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object6;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
}

//���t���[���Ă΂�܂�
void BreakForce::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void BreakForce::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BreakForce::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BreakForce::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void BreakForce::OnCollideExit(GameObject target){
	(void)target;
}