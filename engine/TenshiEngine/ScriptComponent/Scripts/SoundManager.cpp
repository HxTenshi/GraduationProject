#include "SoundManager.h"
#include "../h_standard.h"
#include "../h_component.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Initialize(){
	m_SoundMap[SoundID::Enum::kiru] = sound;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Start(){
}

//���t���[���Ă΂�܂�
void SoundManager::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Finish(){
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SoundManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SoundManager::OnCollideExit(GameObject target){
	(void)target;
}

void SoundManager::GetSound(SoundID::Enum key, XMVECTOR pos){
	if (!soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBox�Ȃ���");
		return;
	}
 	GameObject g = Hx::Instance(soundBox);
	g->mTransform->WorldPosition(pos);
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager�uSoundCommponent�Ȃ���v");
		return;
	}
	s->LoadFile(m_SoundMap[key]);
	s->Play();
}
