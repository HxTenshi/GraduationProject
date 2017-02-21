#include "SoundBox.h"
#include "../h_component.h"
#include "../h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Initialize(){
	m_firstTime = true;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Start(){

}

//���t���[���Ă΂�܂�
void SoundBox::Update(){
	auto sound = gameObject->GetComponent<SoundComponent>();
	if (!sound) {
		Hx::Debug()->Log("SoundBox�usoundComponent�Ȃ���v");
		return;
	}
	//�Đ����I������Ȃ�
	if (!sound->IsPlay() && !m_firstTime) {
		Hx::DestroyObject(gameObject);
	}
	m_firstTime = false;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SoundBox::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundBox::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundBox::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SoundBox::OnCollideExit(GameObject target){
	(void)target;
}