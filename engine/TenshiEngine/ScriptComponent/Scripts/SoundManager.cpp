#include "VolumeBarCtrl.h"
#include "SoundManager.h"
#include "FuckTenshiEngine.h"
#include "../h_standard.h"
#include "../h_component.h"

using namespace Mossan;

//生成時に呼ばれます（エディター中も呼ばれます）
void SoundManager::Initialize(){
	m_SoundSEMap[SoundSE_ID::Enum::kiru] = sound;
	//m_SoundBGMMap[SoundBGM_ID::Enum::test] = nanka;
	//音量のCSVを取得
	std::vector<std::vector<std::string>> arrays = CSVScript::readCSV("Assets/data/Volume.csv");
	bgm_master_volume = std::stof(arrays[0][0]) / VOLUME_RATE;
	se_master_volume = std::stof(arrays[0][1]) / VOLUME_RATE;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void SoundManager::Start(){
}

//毎フレーム呼ばれます
void SoundManager::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void SoundManager::Finish(){
}

//コライダーとのヒット時に呼ばれます
void SoundManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void SoundManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void SoundManager::OnCollideExit(GameObject target){
	(void)target;
}

void SoundManager::GetSoundSE(SoundSE_ID::Enum key, XMVECTOR pos){
	if (!soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
 	GameObject g = Hx::Instance(soundBox);
	g->mTransform->WorldPosition(pos);
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	//コンポーネントの音量とマスターの音量の値を掛ける
	auto volume = s->GetVolume() * se_master_volume;
	s->LoadFile(m_SoundSEMap[key]);
	s->SetVolume(volume);
	s->Set3DSound(true);
	s->SetLoop(false);
	s->Play();
}

void SoundManager::GetSoundBGM(SoundBGM_ID::Enum key){
	if (!soundBox.IsLoad()) {
		Hx::Debug()->Log("soundBoxないよ");
		return;
	}
	GameObject g = Hx::Instance(soundBox);
	g->mTransform->WorldPosition(XMVectorSet(0, 0, 0, 0));
	auto s = g->GetComponent<SoundComponent>();
	if (!s) {
		Hx::Debug()->Log("SoundManager「SoundCommponentないよ」");
		return;
	}

	//コンポーネントの音量とマスターの音量の値を掛ける
	auto volume = s->GetVolume() * bgm_master_volume;
	s->LoadFile(m_SoundBGMMap[key]);
	s->SetVolume(volume);
	s->Set3DSound(false);
	s->SetLoop(true);
	s->Play();
}
