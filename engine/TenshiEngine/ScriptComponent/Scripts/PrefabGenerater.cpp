#include "PrefabGenerater.h"

#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PrefabGenerater::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PrefabGenerater::Start(){

}

//���t���[���Ă΂�܂�
void PrefabGenerater::Update(){
	auto o = Hx::Instance(m_Object);
	if (o) {
		o->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
	}
	gameObject->RemoveComponent<ScriptComponent>();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PrefabGenerater::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PrefabGenerater::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PrefabGenerater::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PrefabGenerater::OnCollideExit(GameObject target){
	(void)target;
}