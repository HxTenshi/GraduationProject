#include "VolumeBarCtrl.h"
#include "SoundManager.h"
#include "FuckTenshiEngine.h"
#include "../h_standard.h"
#include "../h_component.h"

using namespace Mossan;

static SoundManager* g_soundManager = NULL;

//生成時に呼ばれます（エディター中も呼ばれます）
void SoundManager::Initialize(){
	g_soundManager = this;
	//音量のCSVを取得
	std::vector<std::vector<std::string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	bgm_master_volume = std::stof(arrays[0][0]) / VOLUME_RATE;
	se_master_volume = std::stof(arrays[0][1]) / VOLUME_RATE;
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SoundManager::Finish(){
	//g_soundManager = NULL;
}

void SoundManager::PlaySE(SoundSE_ID::Enum key, XMVECTOR pos){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
 	GameObject g = Hx::Instance(g_soundManager->soundBox);
	g->mTransform->WorldPosition(pos);
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	//コンポーネントの音量とマスターの音量の値を掛ける
	auto volume = (s->GetVolume() * g_soundManager->se_master_volume) / 2.0f;
	volume += 0.5f;
	if (g_soundManager->se_master_volume == 0.0f) volume = 0.0f;
	s->LoadFile(g_soundManager->m_soundSEs[(int)key]);
	s->SetVolume(volume);
	s->Set3DSound(true);
	s->SetLoop(false);
	s->Play();
}

void SoundManager::PlaySE(SoundSE_ID::Enum key, float volume){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
	GameObject g = Hx::Instance(g_soundManager->soundBox);
	g->mTransform->WorldPosition(XMVectorZero());
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}


	float temp = (volume / VOLUME_RATE) / 2.0f;
	temp += 0.5f;
	if (volume == 0.0f) volume = 0.0f;
	s->LoadFile(g_soundManager->m_soundSEs[(int)key]);
	s->SetVolume(temp);
	s->Set3DSound(false);
	s->SetLoop(false);
	s->Play();
	float volumea = s->GetVolume();
}

void SoundManager::PlaySE_2D(SoundSE_ID::Enum key)
{
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
	GameObject g = Hx::Instance(g_soundManager->soundBox);
	g->mTransform->WorldPosition(XMVectorZero());
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	//コンポーネントの音量とマスターの音量の値を掛ける
	auto volume = (s->GetVolume() * g_soundManager->se_master_volume) / 2.0f;
	volume += 0.5f;
	if (g_soundManager->se_master_volume == 0.0f) volume = 0.0f;
	s->LoadFile(g_soundManager->m_soundSEs[(int)key]);
	s->SetVolume(volume);
	s->Set3DSound(false);
	s->SetLoop(false);
	s->Play();
}

void SoundManager::PlayBGM(SoundBGM_ID::Enum key){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}

	if (g_soundManager->m_bgmObj == NULL) {
		g_soundManager->m_bgmObj = Hx::Instance(g_soundManager->soundBox);
		Hx::Debug()->Log("BGM Object Instance");
	}
	else {
		Hx::DestroyObject(g_soundManager->m_bgmObj);
		g_soundManager->m_bgmObj = Hx::Instance(g_soundManager->soundBox);
	}
	
	g_soundManager->m_bgmObj->mTransform->WorldPosition(XMVectorSet(0, 0, 0, 0));

	auto s = g_soundManager->m_bgmObj->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	//コンポーネントの音量とマスターの音量の値を掛ける
	auto volume = (s->GetVolume() * g_soundManager->bgm_master_volume) /2.0f;
	volume += 0.5f;
	if (g_soundManager->bgm_master_volume == 0) volume = 0.0f;
	s->LoadFile(g_soundManager->m_soundBGMs[(int)key]);
	s->SetVolume(volume);
	s->Set3DSound(false);
	s->SetLoop(true);
	s->Play();
}

void SoundManager::SetBGMVolume(float volume){
	if (g_soundManager == NULL) {
		Hx::Debug()->Log("SoundManager Null");
		return;
	}
	if (!g_soundManager->soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
	if (g_soundManager->m_bgmObj == NULL) {
		return;
	}
	auto s = g_soundManager->m_bgmObj->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	float temp = (volume / VOLUME_RATE) / 2.0f;
	temp += 0.5f;
	if (volume == 0.0f) temp = 0.0f;
	s->SetVolume(temp);
}
