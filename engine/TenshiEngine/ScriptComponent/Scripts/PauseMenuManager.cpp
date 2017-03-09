#include "PauseMenuManager.h"
#include <h_standard.h>
#include <h_component.h>

#include "UniqueObject.h"
#include "PlayerController.h"
#include "Matinee.h"
#include "OverRayExitOption.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Initialize(){
	m_state = State::Close;
	m_hint = Hint::one;
	m_isItimie = false;
	m_isHint = false;
	m_isHintLeftMove = false;
	m_isHintMoved = false;
	m_arrowTimer = 0.0f;
	rightArrowInitPos = rightArrow->mTransform->Position();
	leftArrowInitPos = leftArrow->mTransform->Position();
	m_num = 0;
	m_stickInterval = 0.0f;
	m_lerpTimers[0] = 0.0f;
	m_lerpTimers[1] = 0.0f;
	m_lerpTimers[2] = 0.0f;
	m_objMap[0].push_back(new Struct(m_texPause,  XMVectorSet(200,900,10001,0)));
	m_objMap[1].push_back(new Struct(m_texTips,   XMVectorSet(960,840,10001,0)));
	m_objMap[1].push_back(new Struct(m_texHowTo,  XMVectorSet(960,640,10001,0)));
	m_objMap[1].push_back(new Struct(m_texCombo,  XMVectorSet(960,440,10001,0)));
	m_objMap[1].push_back(new Struct(m_texToTitle,XMVectorSet(960,240,10001,0)));
	m_objMap[1].push_back(new Struct(m_texMigisita,XMVectorSet(1680,140,10001,0)));
	m_objMap[2].push_back(new Struct(m_texItimie, XMVectorSet(1680,540,10001,0)));
	m_texHintList.push_back(texHintOne);
	m_texHintList.push_back(texHintTwo);
	m_texHintList.push_back(texHintThree);
	m_texHintList.push_back(texHintFour);
	m_texHintList.push_back(texHintFive);

	//this->gameObject->Enable();

	for (int i = 0; i < m_objMap.size(); i++) {
		//左上のテクスチャのみ処理固定
		for (int j = 0; j < m_objMap[i].size(); j++) {
			auto material = m_objMap[i][j]->m_texObj->GetComponent<MaterialComponent>();
			material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
		}
	}
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PauseMenuManager::Start(){
	if (m_overray_exit_option) {
		if (auto src = m_overray_exit_option->GetScript<OverRayExitOption>()) {
			src->InitCall();
			src->SetFuncType(1);
		}
		m_overray_exit_option->Disable();
	}
}

//毎フレーム呼ばれます
void PauseMenuManager::Update() {
	if (m_overray_exit_option->IsEnabled())return;
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
	bool isRightLS = ls.x > 0.5f;//スティック右方向
	bool isLeftLS = ls.x < -0.5f;//スティック左方向
	bool isUpKey = Input::Trigger(KeyCode::Key_UP) || Input::Trigger(PAD_X_KeyCode::Button_UP);		//上キー
	bool isDownKey = Input::Trigger(KeyCode::Key_DOWN) || Input::Trigger(PAD_X_KeyCode::Button_DOWN);//下キー
	bool isRightKey = Input::Trigger(KeyCode::Key_RIGHT) || Input::Trigger(PAD_X_KeyCode::Button_RIGHT);//右キー
	bool isLeftKey = Input::Trigger(KeyCode::Key_LEFT) || Input::Trigger(PAD_X_KeyCode::Button_LEFT);//左キー
	bool isStartKey = Input::Trigger(PAD_X_KeyCode::Button_B);//Bボタン
	bool isPad_A_Key = Input::Trigger(PAD_X_KeyCode::Button_A);//Aボタン

	if (m_isHint) {
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_kakoi->Disable();
		for (auto i : m_texHintList) {
			i->Enable();
		}
		if (m_isHintMoved) {
			rightArrow->Enable();
			leftArrow->Enable();
		}
		else {
			rightArrow->Disable();
			leftArrow->Disable();
		}
		m_arrowTimer += 1.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
		float rightTemp = rightArrowInitPos.x + std::sin(m_arrowTimer) * 100.0f;
		float leftTemp = leftArrowInitPos.x + std::sin(-m_arrowTimer) * 100.0f;
		rightArrow->mTransform->Position(XMVectorSet(rightTemp, rightArrowInitPos.y, rightArrowInitPos.z, 0.0f));
		leftArrow->mTransform->Position(XMVectorSet(leftTemp, leftArrowInitPos.y, leftArrowInitPos.z, 0.0f));


		//ページ戻る
		if ((isRightLS || isRightKey) && m_isHintMoved) {
			m_isHintLeftMove = false;
			m_isHintMoved = false;
			m_preHintTexObj = m_texHintList[m_hint];

			m_hint = (Hint)((int)m_hint - 1);
			if (m_hint < Hint::one) m_hint = Hint::five;
			m_curHintTexObj = m_texHintList[m_hint];

			m_curHintTexObj->mTransform->Position(XMVectorSet(-800, 540, 10001, 0));

			SE(SoundManager::SoundSE_ID::Cursour);
		}
		//ページ進む
		else if((isLeftLS || isLeftKey) && m_isHintMoved) {
			m_isHintLeftMove = true;
			m_isHintMoved = false;
			m_preHintTexObj = m_texHintList[m_hint];

			m_hint = (Hint)((int)m_hint + 1);
			if (m_hint > Hint::five) m_hint = Hint::one;
			m_curHintTexObj = m_texHintList[m_hint];

			m_curHintTexObj->mTransform->Position(XMVectorSet(2800, 540, 10001, 0));

			SE(SoundManager::SoundSE_ID::Cursour);
		}

		if (m_isHintLeftMove) {
			XMVECTOR preObjPos = m_preHintTexObj->mTransform->Position();
			preObjPos.x -= 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (preObjPos.x <= -800.0f) {
				preObjPos.x = -800.0f;
			}
			m_preHintTexObj->mTransform->Position(preObjPos);

			XMVECTOR curObjPos = m_curHintTexObj->mTransform->Position();
			curObjPos.x -= 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (curObjPos.x <= 960.0f) {
				curObjPos.x = 960.0f;
				m_isHintMoved = true;
			}
			m_curHintTexObj->mTransform->Position(curObjPos);
		}
		else {
			XMVECTOR preObjPos = m_preHintTexObj->mTransform->Position();
			preObjPos.x += 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (preObjPos.x >= 2800.0f) {
				preObjPos.x = 2800.0f;
			}
			m_preHintTexObj->mTransform->Position(preObjPos);

			XMVECTOR curObjPos = m_curHintTexObj->mTransform->Position();
			curObjPos.x += 4000.0f * Hx::DeltaTime()->GetNoScaleDeltaTime();
			if (curObjPos.x >= 960.0f) {
				curObjPos.x = 960.0f;
				m_isHintMoved = true;
			}
			m_curHintTexObj->mTransform->Position(curObjPos);
		}



		//めにゅーを閉じる
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey || isPad_A_Key) MenuReAction(5);
	}
	else if (!m_isItimie) {
		//lerpの計算
		m_lerpTimers[1] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		
		
		if (isUpLS && isStickInterval) { 
			if (m_num <= 0) return;
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownLS && isStickInterval) { 
			if (m_num >= 4) return;
			m_num++; 
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (isUpKey) {
			if (m_num <= 0) return;
			m_num--;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}
		if (isDownKey) {
			if (m_num >= 4) return;
			m_num++;
			m_stickInterval = 0.0f;
			SE(SoundManager::SoundSE_ID::Enum::Cursour);
		}

		if (m_lerpTimers[1] >= 1.0f) {
			if (isPad_A_Key) ClosePauseMenu();
		}

		//0～3の間にクランプ
		m_num = min(max(0, m_num), 4);
		
		//それぞれの反応を起こす
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey) MenuReAction(m_num);

		//囲い有効,座標移動
		m_kakoi->Enable();
		XMVECTOR pos = m_objMap[1][m_num]->m_texObj->mTransform->Position();
		pos.z += 1.0f;
		m_kakoi->mTransform->Position(pos);

		for (auto i : m_texHintList) {
			i->Disable();
		}
	}
	else {
		//lerpの計算
		m_lerpTimers[1] -= m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_lerpTimers[2] += m_lerpSpeed * Hx::DeltaTime()->GetNoScaleDeltaTime();

		//めにゅーを閉じる
		if (Input::Trigger(KeyCode::Key_SPACE) || isStartKey || isPad_A_Key) MenuReAction(5); 

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
	m_Camvas->Enable();
	m_state = State::Open;
	m_isItimie = false;
	m_isHint = false;
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
			pc->SetPlayerLock(true);
		}
	}

	rightArrow->Disable();
	leftArrow->Disable();
}

//ポーズメニューを閉じる
void PauseMenuManager::ClosePauseMenu(){
	Hx::DeltaTime()->SetTimeScale(1.0f);
	m_state = State::Close;
	m_isHint = false;
	rightArrow->Disable();
	leftArrow->Disable();

	if (auto player = UniqueObject::GetPlayer()) {
		if (auto pc = player->GetScript<PlayerController>()) {
			pc->SetPlayerLock(false);
		}
	}

	for (auto i : m_texHintList) {
		i->Disable();
	}
}

//それぞれの処理を呼ぶ
void PauseMenuManager::MenuReAction(int num){
	//決定音声再生
	SE(SoundManager::SoundSE_ID::Enum::Decision);

	switch (num)
	{
	case 0:	OnHint();
		break;
	case 1:	ChangeItimaie(Itimie::HowTo);	//操作方法に切り替える
		break;
	case 2:	ChangeItimaie(Itimie::Combo);	//コンボ表に切り替える
		break;
	case 3: BackToTitle();		//タイトルに飛ぶ
		break;
	case 4: ClosePauseMenu();	//メニューを閉じる
		break;
	case 5: BackMenu();			//1枚絵からメニューに戻る
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
	switch (type){
	case PauseMenuManager::HowTo: texItimaie = texHowTo;
		break;
	case PauseMenuManager::Combo: texItimaie = texCombo;
		break;
	}

	auto material1 = m_objMap[2][0]->m_texObj->GetComponent<MaterialComponent>();
	material1->GetMaterialPtr(0)->SetTexture(texItimaie);
	auto material2 = m_objMap[1][4]->m_texObj->GetComponent<MaterialComponent>();
	material2->GetMaterialPtr(0)->SetTexture(texBack);

}

//1枚絵からメニューリストに戻る
void PauseMenuManager::BackMenu(){
	if (m_objMap[1][3] == nullptr) return;
	m_isItimie = false;
	m_isHint = false;
	rightArrow->Disable();
	leftArrow->Disable();
	auto material = m_objMap[1][4]->m_texObj->GetComponent<MaterialComponent>();
	material->GetMaterialPtr(0)->SetTexture(texClose);
}

//タイトルに飛ぶ
void PauseMenuManager::BackToTitle(){
	if (!m_overray_exit_option->IsEnabled()) {
		m_overray_exit_option->Enable();
		m_overray_exit_option->GetScript<OverRayExitOption>()->SetOverrayFlag(true);

	}
	//Hx::DeltaTime()->SetTimeScale(1.0f);
	//Hx::LoadScene("Assets/Title.scene");
}

void PauseMenuManager::OnHint(){
	m_hint = Hint::one;
	m_isHint = true;
	m_isHintLeftMove = true;
	m_preHintTexObj = m_texHintList[Hint::five];
	m_curHintTexObj = m_texHintList[Hint::one];

	for (auto i : m_texHintList) {
		i->mTransform->Position(XMVectorSet(2800,540,10001,0));
	}

	m_preHintTexObj->mTransform->Position(XMVectorSet(-600, 540,10001,0));
	m_curHintTexObj->mTransform->Position(XMVectorSet(2800, 540,10001,0));
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
	SoundManager::PlaySE_2D(key);
}
