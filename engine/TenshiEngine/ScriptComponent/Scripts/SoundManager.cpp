#include "VolumeBarCtrl.h"
#include "SoundManager.h"
#include "FuckTenshiEngine.h"
#include "../h_standard.h"
#include "../h_component.h"

using namespace Mossan;

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Initialize(){
	m_SoundSEMap[SoundSE_ID::Enum::kiru] = sound;
	//m_SoundBGMMap[SoundBGM_ID::Enum::test] = nanka;
	//���ʂ�CSV���擾
	std::vector<std::vector<std::string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	bgm_master_volume = std::stof(arrays[0][0]) / VOLUME_RATE;
	se_master_volume = std::stof(arrays[0][1]) / VOLUME_RATE;
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

void SoundManager::GetSoundSE(SoundSE_ID::Enum key, XMVECTOR pos){
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

	//�R���|�[�l���g�̉��ʂƃ}�X�^�[�̉��ʂ̒l���|����
	auto volume = s->GetVolume() * se_master_volume;
	s->LoadFile(m_SoundSEMap[key]);
	s->SetVolume(volume);
	s->Set3DSound(true);
	s->SetLoop(false);
	s->Play();
}

void SoundManager::GetSoundBGM(SoundBGM_ID::Enum key){
	if (!soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBox�Ȃ���");
		return;
	}
	GameObject g = Hx::Instance(soundBox);
	g->mTransform->WorldPosition(XMVectorSet(0, 0, 0, 0));
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager�uSoundCommponent�Ȃ���v");
		return;
	}

	//�R���|�[�l���g�̉��ʂƃ}�X�^�[�̉��ʂ̒l���|����
	auto volume = s->GetVolume() * bgm_master_volume;
	s->LoadFile(m_SoundBGMMap[key]);
	s->SetVolume(volume);
	s->Set3DSound(false);
	s->SetLoop(true);
	s->Play();
}
