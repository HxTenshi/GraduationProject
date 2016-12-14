#include "SoundManager.h"
#include "../h_standard.h"
#include "../h_component.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void SoundManager::Initialize(){
	m_SoundMap[SoundID::Enum::kiru] = sound;
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

void SoundManager::GetSound(SoundID::Enum key, XMVECTOR pos){

 	GameObject g = Hx::Instance(soundBox);
	g->mTransform->WorldPosition(pos);
	auto s = g->GetComponent<SoundComponent>();
	s->LoadFile(m_SoundMap[key]);
	s->Play();
}
