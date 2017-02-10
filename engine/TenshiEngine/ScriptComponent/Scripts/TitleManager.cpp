#include "TitleManager.h"

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

	}

	//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
	void TitleManager::Start() {
		iup_flag = true;
	}

	//毎フレーム呼ばれます
	void TitleManager::Update() {

		if (Input::Trigger(KeyCode::Key_A) && !is_next) {
			is_next = true;
			is_arrow = false;
			InitPosition(is_arrow);
			Back();
			SetArrowPosition();
		}
		if (Input::Trigger(KeyCode::Key_D) && !is_next) {
			is_next = true;
			is_arrow = true;
			InitPosition(is_arrow);
			Next();
			SetArrowPosition();

		}
		if (Input::Trigger(KeyCode::Key_SPACE)) {
			Select();
		}
		if (is_next) {

			m_MoveObject.Move(is_arrow);
			m_NowObject.Move(is_arrow);
			if (ArrowOver()) {
				is_next = false;
			}
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
		switch (m_button_state)
		{
		case TitleButtonRoll::GameStart: //Hx::LoadScene("");
			break;
		case TitleButtonRoll::Config: Hx::LoadScene("Assets/Mossan/Config.scene");
			break;
		case TitleButtonRoll::GameEnd: //Hx::LoadScene("");
			break;
		default:
			break;
		}
	}