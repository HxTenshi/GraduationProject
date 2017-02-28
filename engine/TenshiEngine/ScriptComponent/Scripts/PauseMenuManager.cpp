#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

#include "UniqueObject.h"
#include "PlayerController.h"
#include "Matinee.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_isItimie = false;
	m_num = 0;
	m_stickInterval = 0.0f;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,10001,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,740,10001,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,540,10001,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,340,10001,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,10001,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,10001,0)));
	this->gameObject->Enable();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Start(){

}

//毎フレーム呼ばれます
void PauseMenuManager::Update() {
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_START);
	if (Input::Trigger(KeyCode::Key_M) || isStartKey) {
		if(m_state == State::Close) OpenPauseMenu();
		else if (m_state == State::Open) ClosePauseMenu();
	}
	if (m_state == State::Open) { 
		Hx::DeltaTime()->SetTimeScale(0.0f);
		UpdateOpne(); 
	}
	else if (m_state == State::Close) { 
		UpdateClose(); 
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PauseMenuManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PauseMenuManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PauseMenuManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PauseMenuManager::OnCollideExit(GameObject target){
	(void)target;
}

//開いている間の処理
void PauseMenuManager::UpdateOpne() {

	//リターン処理
	for (int i = 0; i < m_objMap.size(); i++)
		for (int j = 0; j < m_objMap[i].size(); j++)
			if (m_objMap[i][j] == nullptr) return;

	//入力間隔
	const float STICK_INTERVAL = 0.15f;
	//インターバル加算
	m_stickInterval += 1.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
	//上限設定
	m_stickInterval = min(m_stickInterval, STICK_INTERVAL);
	//入力間隔を見たいしているか
	bool isStickInterval = m_stickInterval >= STICK_INTERVAL;


	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);	//左スティック
	bool isUpLS = ls.y > 0.5f;	//スティック上方向
	bool isDownLS = ls.y < -0.5f;//スティック下方向
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_B);//Bボタン

	if (!m_isItimie) {
		//lerpの計算
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		
		
		if (isUpLS && isStickInterval) { 
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownLS && isStickInterval) { 
			m_num++; 
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (Input::Trigger(KeyCode::Key_UP)) {
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (Input::Trigger(KeyCode::Key_DOWN)) {
			m_num++;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		//0～3の間にクランプ
		m_num = min(max(0, m_num), 3);

		
		//それぞれの反応を起こす
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(m_num);

		//囲い有効,座標移動
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		pos.z += 1.0f;
		m_kakoi->mTransform->Position(pos);
	}
	else {
		//lerpの計算
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

		//めにゅーを閉じる
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(4); 

		//囲い無効
		m_kakoi->Disable();
	}

	//左上のポーズテクスチャ
	m_lerpTimers[0] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

	//クランプ処理
	m_lerpTimers[0] = min(max(0.0f, m_lerpTimers[0]), 1.0f);
	m_lerpTimers[1] = min(max(0.0f, m_lerpTimers[1]), 1.0f);
	m_lerpTimers[2] = min(max(0.0f, m_lerpTimers[2]), 1.0f);


	/*座標移動*/
	LerpFunc(m_objMap[1][3], 1.0f);
	for (int i = 0; i < m_objMap.size(); i++)
		//左上のテクスチャのみ処理固定
		for (int j = 0; j < m_objMap[i].size(); j++)
			LerpFunc(m_objMap[i][j], m_lerpTimers[i]);
	//右下のテクスチャ
	LerpFunc(m_objMap[0][0], m_lerpTimers[0]);


	//フェードイン
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
	if (color.w >= 0.8f) color.w = 0.8f;
	material->SetAlbedoColor(color);
}

//閉じている間の処理
void PauseMenuManager::UpdateClose(){
	m_kakoi->Disable();
	for (int i = 0; i < m_objMap.size(); i++) {
		for (int j = 0; j < m_objMap[i].size(); j++) {
			//左にスライド移動
			XMVECTOR tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
			tempPos.x -= 100.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			//x座標が0を下回らないように処理
			if (m_objMap[i][j]->m_texObj->mTransform->Position().x < 0.0f) {
				tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
				tempPos.x = 0.0f;
			}
			m_objMap[i][j]->m_texObj->mTransform->Position(tempPos);
			//マテリアルのカラー　アルファー値減算
			auto mat = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			XMFLOAT4 tempColor = mat->GetMaterialPtr(0)->GetAlbedo();
			tempColor.w -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (tempColor.w <= 0.0f) tempColor.w = 0.0f;
			mat->SetAlbedoColor(tempColor);
		}
	}

	//フェードアウト
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
	if (color.w <= 0.0f) color.w = 0.0f;
	material->SetAlbedoColor(color);

}

//ポーズメニューを開いた際の処理
void PauseMenuManager::OpenPauseMenu(){
	if (Matinee::GlobalNowPlaying()) {
		return;
	}
	m_state = State::Open;
	m_isItimie = false;
	m_num = 0;
	for (int i = 0; i < m_objMap.size(); i++) {
		m_lerpTimers[i] = 0.0f;
		for (int j = 0; j < m_objMap[i].size(); j++) {
			//初期座標と移動後の座標をセット
			m_objMap[i][j]->m_pos1 = m_objMap[i][j]->m_pivotPos + XMVectorSet(-600, 0, 0, 0);
			m_objMap[i][j]->m_pos2 = m_objMap[i][j]->m_pivotPos;
			if (i == 2) {
				//一枚絵の初期座標と移動後の座標をセット
				m_objMap[i][j]->m_pos1 = m_objMap[i][j]->m_pivotPos;
				m_objMap[i][j]->m_pos2 = m_objMap[i][j]->m_pivotPos + XMVectorSet(-600, 0, 0, 0);
			}
			//マテリアルのカラー　アルファー値減算
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			material->SetAlbedoColor(XMFLOAT4(1, 1, 1, 0));
		}
	}
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
	m_texBlack->Enable();

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerState(PlayerController::PlayerState::Movie);
		}
	}
}

//ポーズメニューを閉じる
void PauseMenuManager::ClosePauseMenu(){
	Hx::DeltaTime()->SetTimeScale(1.0f);
	m_state = State::Close;

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerState(PlayerController::PlayerState::Free);
		}
	}
}

//それぞれの処理を呼ぶ
void PauseMenuManager::MenuReAction(int num){
	//決定音声再生
	SE(SoundManager::SoundSE_ID::Enum::Decision);

	switch (num)
	{
	case 0:	ChangeItimaie(Itimie::HowTo);	//操作方法に切り替える
		break;
	case 1:	ChangeItimaie(Itimie::Combo);	//コンボ表に切り替える
		break;
	case 2:	BackToTitle();		//タイトルに飛ぶ
		break;
	case 3: ClosePauseMenu();	//メニューを閉じる
		break;
	case 4: BackMenu();			//1枚絵からメニューに戻る
		break;
	default:
		break;
	}
}

//対応した一枚絵の切り替える
void PauseMenuManager::ChangeItimaie(Itimie type){
	if (m_objMap[2][0] == nullptr) return;
	if (m_objMap[1][3] == nullptr) return;

	m_isItimie = true;
	TextureAsset texItimaie;
	switch (type)
	{
	case PauseMenuManager::HowTo: texItimaie = texHowTo;
		break;
	case PauseMenuManager::Combo: texItimaie = texCombo;
		break;
	}
	auto material1 = m_objMap[2][0]->m_texObj->GetComponent<MaterialComponent>();
	material1->GetMaterialPtr(0)->SetTexture(texItimaie);
	auto material2 = m_objMap[1][3]->m_texObj->GetComponent<MaterialComponent>();
	material2->GetMaterialPtr(0)->SetTexture(texBack);

}

//1枚絵からメニューリストに戻る
void PauseMenuManager::BackMenu(){
	if (m_objMap[1][3] == nullptr) return;
	m_isItimie = false;
	auto material = m_objMap[1][3]->m_texObj->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetTexture(texClose);
}

//タイトルに飛ぶ
void PauseMenuManager::BackToTitle(){
	Hx::DeltaTime()->SetTimeScale(1.0f);
	Hx::LoadScene("Assets/Title.scene");
}

//Lerpを用いた処理
void PauseMenuManager::LerpFunc(Struct* structObj, float lerpTime){
	if (structObj == nullptr) return;
	//クランプ処理
	lerpTime = min(max(0.0f, lerpTime), 1.0f);
	//座標移動
	XMVECTOR tempPos = XMVectorLerp(structObj->m_pos1, structObj->m_pos2, lerpTime);
	structObj->m_texObj->mTransform->Position(tempPos);
	//マテリアルのアルファ値の計算
	auto material = structObj->m_texObj->GetComponent<MaterialComponent>();
	material->SetAlbedoColor(XMFLOAT4(1, 1, 1, lerpTime));
}

void PauseMenuManager::SE(SoundManager::SoundSE_ID::Enum key){
	XMVECTOR cameraPos = UniqueObject::GetCamera()->mTransform->Position();
	SoundManager::PlaySE(key, cameraPos);
}
