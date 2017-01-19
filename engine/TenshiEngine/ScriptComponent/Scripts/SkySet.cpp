#include "SkySet.h"

#include "h_component.h"

#include "UniqueObject.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SkySet::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SkySet::Start(){

}

//���t���[���Ă΂�܂�
void SkySet::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SkySet::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SkySet::OnCollideBegin(GameObject target){
	
	if (UniqueObject::IsPlayer(target)) {
		if (auto obj = UniqueObject::GetCamera()) {
			if (auto cam = obj->GetComponent<CameraComponent>()) {
				cam->SetSkyTexture(m_SkyTexture);
			}
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SkySet::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SkySet::OnCollideExit(GameObject target){
	(void)target;
}