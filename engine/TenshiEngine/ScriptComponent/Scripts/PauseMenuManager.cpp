#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

//生成時に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_isItimie = false;
	m_num = 0;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,0,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,740,0,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,540,0,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,340,0,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,0,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,0,0)));
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Start(){

}

//毎フレーム呼ばれます
void PauseMenuManager::Update(){
	if (Input::Trigger(KeyCode::Key_M)) {
		OpenPauseMenu();
	}
	if (m_state == State::Open) UpdateOpne();
	else if (m_state == State::Close) UpdateClose();
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

	//Lerp処理
	for (int i = 0; i < m_objMap.size(); i++) {
		//左上のテクスチャのみ処理固定
		for (int j = 0; j < m_objMap[i].size(); j++) {
			if (m_objMap[i][j] == nullptr) return;
		}
	}

	//左上のポーズテクスチャ
	m_lerpTimers[0] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
	LerpFunc(m_objMap[0][0], m_lerpTimers[0]);

	if (!m_isItimie) {
		//lerpの計算
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		//0～3の間にクランプ
		if (Input::Trigger(KeyCode::Key_UP)) m_num--;
		if (Input::Trigger(KeyCode::Key_DOWN)) m_num++;
		m_num = min(max(0, m_num), 3);

		//それぞれの反応を起こす
		if (Input::Trigger(KeyCode::Key_SPACE)) MenuReAction(m_num);

		//囲い有効,座標移動
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		m_kakoi->mTransform->Position(pos);
	}
	else {
		//lerpの計算
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();

		//めにゅーを閉じる
		if (Input::Trigger(KeyCode::Key_SPACE))MenuReAction(4);

		//囲い無効
		m_kakoi->Disable();
	}

	//Lerp処理
	for (int i = 0; i < m_objMap.size(); i++) {
		//左上のテクスチャのみ処理固定
		for (int j = 0; j < m_objMap[i].size(); j++) {
			LerpFunc(m_objMap[i][j], m_lerpTimers[i]);
		}
	}
	//右下のテクスチャ
	LerpFunc(m_objMap[1][3], 1.0f);


	//フェードイン
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w += m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
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
			tempPos.x -= 100.0f * Hx::DeltaTime()->GetDeltaTime();
			//x座標が0を下回らないように処理
			if (m_objMap[i][j]->m_texObj->mTransform->Position().x < 0.0f) {
				tempPos = m_objMap[i][j]->m_texObj->mTransform->Position();
				tempPos.x = 0.0f;
			}
			m_objMap[i][j]->m_texObj->mTransform->Position(tempPos);
			//マテリアルのカラー　アルファー値減算
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			XMFLOAT4 tempColor = material->GetMaterialPtr(0)->GetAlbedo();
			tempColor.w -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
			material->SetAlbedoColor(tempColor);
		}
	}

	//フェードアウト
	auto material = m_texBlack->GetComponent<MaterialComponent>();
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();
	color.w -= m_lerpSpeed * Hx::DeltaTime()->GetDeltaTime();
	if (color.w <= 0.0f) color.w = 0.0f;
	material->SetAlbedoColor(color);
}

//ポーズメニューを開いた際の処理
void PauseMenuManager::OpenPauseMenu(){
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
}

//ポーズメニューを閉じる
void PauseMenuManager::ClosePauseMenu(){
	m_state = State::Close;
}

//それぞれの処理を呼ぶ
void PauseMenuManager::MenuReAction(int num){

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
	Hx::LoadScene("Assets/TitleScene.scene");
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
