#include "TitleManager.h"
#include "Fader.h"

namespace funifuni {


	TitleMoveButton::TitleMoveButton(GameObject obj)
	{
		SetGameObject(obj);
	}
	TitleMoveButton::TitleMoveButton()
	{
	}
	void TitleMoveButton::SetGameObject(GameObject obj) {
		this->obj = obj;
	}
	GameObject TitleMoveButton::GetObject() {
		return obj;
	}
	void TitleMoveButton::Move(bool arrow) {
		XMVECTOR pos = obj->mTransform->Position();
		if (arrow) {
			pos.x += 3000.0f*Hx::DeltaTime()->GetDeltaTime();
		}
		else {
			pos.x -= 3000.0f*Hx::DeltaTime()->GetDeltaTime();
		}
		obj->mTransform->Position(pos);
	}
	void TitleMoveButton::SetArrowPos(bool arrow) {
		if (arrow) {
			XMVECTOR pos = obj->mTransform->Position();
			pos.x = -1040;
			obj->mTransform->Position(pos);

		}
		else {
			XMVECTOR pos = obj->mTransform->Position();
			pos.x = 3000;
			obj->mTransform->Position(pos);
		}
	}

}

//生成時に呼ばれます（エディター中も呼ばれます）
	void TitleManager::Initialize() {
		m_Buttons.push_back(funifuni::TitleMoveButton(m_GameStartButton));
		m_Buttons.push_back(funifuni::TitleMoveButton(m_ConfigButton));
		m_Buttons.push_back(funifuni::TitleMoveButton(m_GameEndButton));
		m_button_x = 960;
		m_button_y = 200;
		m_button_state = GameStart;
		m_MoveObject = m_GameStartButton;
		is_next = false;
		is_bgm = false;
	}

	//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
	void TitleManager::Start() {
		iup_flag = true;
		m_state = State::state1;
		selectNum = 0;
		m_intervalTime = 0.0f;
		OnBGM();
	}

	//毎フレーム呼ばれます
	void TitleManager::Update() {
		
		//入力関連を取得
		auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
		bool isRightLS = ls.x > 0.5f;	//右に倒したか？
		bool isLeftLS = ls.x < -0.5f;	//左に倒したか？
		bool isEnter = Input::Trigger(KeyCode::Key_SPACE) || Input::Trigger(PAD_X_KeyCode::Button_B);	//決定
		bool isCansel = Input::Trigger(KeyCode::Key_B) || Input::Trigger(PAD_X_KeyCode::Button_A);		//キャンセル

		if (m_state == State::state1) {

			m_canvas1->Enable();
			m_canvas2->Disable();

			if ((Input::Trigger(KeyCode::Key_LEFT) || isLeftLS) && !is_next) {
				is_next = true;
				is_arrow = false;
				InitPosition(is_arrow);
				Back();
				SetArrowPosition();
				OnSE(SoundManager::SoundSE_ID::Enum::Cursour);
			}
			if ((Input::Trigger(KeyCode::Key_RIGHT) || isRightLS) && !is_next) {
				is_next = true;
				is_arrow = true;
				InitPosition(is_arrow);
				Next();
				SetArrowPosition();
				OnSE(SoundManager::SoundSE_ID::Enum::Cursour);
			}

			if (isEnter) Select();

			if (is_next) {
				m_MoveObject.Move(is_arrow);
				m_NowObject.Move(is_arrow);
				if (ArrowOver()) {
					is_next = false;
				}
			}
		}
		else if (m_state == State::state2) {

			m_canvas1->Disable();
			m_canvas2->Enable();

			const float IntervalTime = 0.3f;
			m_intervalTime += 1.0f * Hx::DeltaTime()->GetDeltaTime();
			m_intervalTime = min(m_intervalTime, IntervalTime);
			bool isInterval = m_intervalTime >= IntervalTime;
			bool isBool = (isLeftLS || isRightLS) && isInterval;

			if (Input::Trigger(KeyCode::Key_LEFT) || Input::Trigger(KeyCode::Key_RIGHT) || isBool) {
				m_intervalTime = 0.0f;
				selectNum++;
				OnSE(SoundManager::SoundSE_ID::Enum::Cursour);
			}

			int temp = selectNum % 2;

			XMVECTOR kakoiPos = XMVectorSet(660.0f + 500.0f * temp, 440.0f,0.0f,0.0f);
			m_kakoi->mTransform->Position(kakoiPos);

			//決定
			if (isEnter) Select2(temp);
			//キャンセル
			if (isCansel) m_state = State::state1;
		}
	}

	//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
	void TitleManager::Finish() {

	}

	//コライダーとのヒット時に呼ばれます
	void TitleManager::OnCollideBegin(GameObject target) {
		(void)target;
	}

	//コライダーとのヒット中に呼ばれます
	void TitleManager::OnCollideEnter(GameObject target) {
		(void)target;
	}

	//コライダーとのロスト時に呼ばれます
	void TitleManager::OnCollideExit(GameObject target) {
		(void)target;
	}
	void TitleManager::InitPosition(bool arrow)
	{
		if (iup_flag) {
			iup_flag = false;
			if (arrow) {
				if (arrow) {
					XMVECTOR pos = m_ConfigButton->mTransform->Position();
					XMVECTOR pos2 = m_GameEndButton->mTransform->Position();
					pos.x = -1040;
					pos2.x = -1040;
					m_ConfigButton->mTransform->Position(pos);
					m_GameEndButton->mTransform->Position(pos);
				}
				else {
					XMVECTOR pos = m_ConfigButton->mTransform->Position();
					XMVECTOR pos2 = m_GameEndButton->mTransform->Position();
					pos.x = 3000;
					pos2.x = 3000;
					m_ConfigButton->mTransform->Position(pos);
					m_GameEndButton->mTransform->Position(pos);
				}
			}
			else {
			}
			
		}
	}
	//ポジションの制御
	bool TitleManager::ArrowOver()
	{
		if (!m_MoveObject.GetObject())return false;

		XMVECTOR pos = m_MoveObject.GetObject()->mTransform->Position();
		if (is_arrow) {
			if (m_button_x < pos.x) {
				pos.x = m_button_x;
				m_MoveObject.GetObject()->mTransform->Position(pos);
				return true;
			}
		}
		else {
			if (m_button_x > m_MoveObject.GetObject()->mTransform->Position().x) {
				pos.x = m_button_x;
				m_MoveObject.GetObject()->mTransform->Position(pos);
				return true;
			}
		}
		return false;
	}

	void TitleManager::Back()
	{
		m_button_state = (TitleButtonRoll)((int)m_button_state - 1);
		if (m_button_state == TitleButtonRoll::Start)m_button_state = TitleButtonRoll::GameEnd;
		SetMoveObject();
	}

	void TitleManager::Next()
	{
		m_button_state = (TitleButtonRoll)((int)m_button_state + 1);
		if (m_button_state == TitleButtonRoll::End)m_button_state = TitleButtonRoll::GameStart;
		SetMoveObject();
	}

	void TitleManager::SetMoveObject()
	{
		switch (m_button_state) {
		case TitleButtonRoll::GameStart:
			m_NowObject = m_MoveObject;
			m_MoveObject = m_GameStartButton;

			break;
		case TitleButtonRoll::Config:
			m_NowObject = m_MoveObject;
			m_MoveObject = m_ConfigButton;
			break;
		case TitleButtonRoll::GameEnd:
			m_NowObject = m_MoveObject;
			m_MoveObject = m_GameEndButton;
			break;
		}
	}
	//1240
	//-280
	void TitleManager::SetArrowPosition()
	{
		for (auto i : m_Buttons)
		{
			if (i.GetObject() != m_NowObject.GetObject()) {
				if (i.GetObject() != m_MoveObject.GetObject()) {
					i.SetArrowPos(is_arrow);
				}
			}

		}
	}

	void TitleManager::Select()
	{
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();

		switch (m_button_state)
		{
		case TitleButtonRoll::GameStart: m_state = State::state2;//fader->OnSceneChnage(gameStartScenePass);
			break;
		case TitleButtonRoll::Config: fader->OnSceneChnage(configScenePass);
			break;
		case TitleButtonRoll::GameEnd: Hx::Shutdown();
			break;
		default:
			break;
		}
	}

	void TitleManager::Select2(int num){
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();

		switch (num)
		{
		case 0:	fader->OnSceneChnage(turorialScenePass);
			break;
		case 1:	fader->OnSceneChnage(gameStartScenePass);
			break;
		default:
			break;
		}
	}

	void TitleManager::OnBGM(){
		SoundManager::PlayBGM(SoundManager::SoundBGM_ID::Enum::Title);
	}

	void TitleManager::OnSE(SoundManager::SoundSE_ID::Enum seID)
	{
		SoundManager::PlaySE(seID, XMVectorSet(-4.0f,17.0f,-3.0f,0.0f));
	}
