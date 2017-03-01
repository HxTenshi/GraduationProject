#include "GameOverScene.h"
#include "Fader.h"
#include "SoundManager.h"
#include <h_component.h>
#include <h_standard.h>


//生成時に呼ばれます（エディター中も呼ばれます）
void GameOverScene::Initialize(){
	if (m_texRogo == NULL) return;
	if (m_texMessage == NULL) return;

	m_intervalTime = 0.0f;
	m_tempTime = 0.0f;

	auto material1 = m_texRogo->GetComponent<MaterialComponent>();
	material1->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1,1,1,0));
	auto material2 = m_texMessage->GetComponent<MaterialComponent>();
	material2->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameOverScene::Start(){
	SoundManager::PlayBGM(SoundManager::SoundBGM_ID::Enum::GameOver);
}

//毎フレーム
void GameOverScene::Update(){
	if (m_texRogo == NULL) return;
	if (m_texMessage == NULL) return;
	bool isRogo = false;
	bool isIntarval = false;
	bool isMessage = false;

	//ゲームオーバーのロゴを表示
	auto material1 = m_texRogo->GetComponent<MaterialComponent>();
	XMFLOAT4 color1 = material1->GetMaterialPtr(0)->GetAlbedo();
	color1.w += 0.5f * Hx::DeltaTime()->GetDeltaTime();
	if (color1.w > 1.0f) { 
		isRogo = true;
		color1.w = 1.0f;
	}
	material1->GetMaterialPtr(0)->SetAlbedo(color1);

	//ロゴが表示終わったなら
	if (isRogo) {
		//0.5秒の間を取る
		m_intervalTime += 1.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_intervalTime > 0.5f) {
			isIntarval = true;
			m_intervalTime = 0.5f;
		}
	}

	//間を取ったなら
	if (isIntarval) {
		//メッセージを表示する
		auto material2 = m_texMessage->GetComponent<MaterialComponent>();
		XMFLOAT4 color2 = material2->GetMaterialPtr(0)->GetAlbedo();
		color2.w += 0.5f * Hx::DeltaTime()->GetDeltaTime();
		if (color2.w > 1.0f) {
			color2.w = 1.0f;
			isMessage = true;
		}
		material2->GetMaterialPtr(0)->SetAlbedo(color2);
	}

	//メッセージが表示完了したなら
	if (isMessage) {
		const float TEMP = 5.0f;
		m_tempTime += 1.0f * Hx::DeltaTime()->GetDeltaTime();
		m_tempTime = min(m_tempTime, TEMP);


		if (m_tempTime >= TEMP) {
			if (!m_fader) return;
			auto fader = m_fader->GetScript<Fader>();
			fader->OnSceneChnage("Assets/Mossan/Credit.scene");
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameOverScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GameOverScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameOverScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameOverScene::OnCollideExit(GameObject target){
	(void)target;
}