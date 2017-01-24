#include "GameOverManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void GameOverManager::Initialize(){
	m_time = 0;
	m_telop->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1, 1, 1, 0));

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameOverManager::Start(){
	
	
}

//毎フレーム呼ばれます
void GameOverManager::Update(){
	//m_material.SetAlbedoColor(XMFLOAT4(1, 1, 1, m_time*2.0f));
	ViewTelop();
	m_telop->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1, 1, 1, m_time*2.0f));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameOverManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GameOverManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameOverManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameOverManager::OnCollideExit(GameObject target){
	(void)target;
}

void GameOverManager::ViewTelop()
{
	m_time += Hx::DeltaTime()->GetDeltaTime()*0.01f;
	if (m_time > 0.5f)m_time = 0.5f;

}
