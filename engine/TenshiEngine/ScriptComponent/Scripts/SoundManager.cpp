#include "VolumeBarCtrl.h"
#include "SoundManager.h"
#include "FuckTenshiEngine.h"
#include "../h_standard.h"
#include "../h_component.h"

using namespace Mossan;

static SoundManager* g_soundManager = NULL;

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Initialize(){
	g_soundManager = this;
	//���ʂ�CSV���擾
	std::vector<std::vector<std::string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	bgm_master_volume = std::stof(arrays[0][0]) / VOLUME_RATE;
	se_master_volume = std::stof(arrays[0][1]) / VOLUME_RATE;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SoundManager::Finish(){
	//g_soundManager = NULL;
}

void SoundManager::PlaySE(SoundSE_ID::Enum key, XMVECTOR pos){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBox�Ȃ���");
		return;
	}
 	GameObject g = Hx::Instance(g_soundManager->soundBox);
	g->mTransform->WorldPosition(pos);
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager�uSoundCommponent�Ȃ���v");
		return;
	}

	//�R���|�[�l���g�̉��ʂƃ}�X�^�[�̉��ʂ̒l���|����
	auto volume = s->GetVolume() * g_soundManager->se_master_volume;
	s->LoadFile(g_soundManager->m_soundSEs[(int)key]);
	s->SetVolume(volume);
	s->Set3DSound(true);
	s->SetLoop(false);
	//s->Play();
}

void SoundManager::PlayBGM(SoundBGM_ID::Enum key){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBox�Ȃ���");
		return;
	}
	GameObject g = Hx::Instance(g_soundManager->soundBox);
	g->mTransform->WorldPosition(XMVectorSet(0, 0, 0, 0));
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager�uSoundCommponent�Ȃ���v");
		return;
	}

	//�R���|�[�l���g�̉��ʂƃ}�X�^�[�̉��ʂ̒l���|����
	auto volume = s->GetVolume() * g_soundManager->bgm_master_volume;
	s->LoadFile(g_soundManager->m_soundBGMs[(int)key]);
	s->SetVolume(volume);
	s->Set3DSound(false);
	s->SetLoop(true);
	//s->Play();
}
