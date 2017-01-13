
#pragma once
#include "main.h"
#include <vector>
#include "h_standard.h"
enum TitleButtonRoll {
	Start = 0,
	GameStart,
	Config,
	GameEnd,
	End
};
namespace funifuni {
	class TitleMoveButton {
	public:
		TitleMoveButton(GameObject obj);
		TitleMoveButton();
		void SetGameObject(GameObject obj);
		GameObject GetObject();
		void Move(bool arrow);
		void SetArrowPos(bool arrow);
	private:
		GameObject obj;

	};

};

class TitleManager :public IDllScriptComponent {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	//�w��̈ʒu���I�[�o�[������
	bool ArrowOver();
	void Back();
	void Next();
	void SetMoveObject();
	void SetArrowPosition();
	void Select();
private:
	//�����o�ϐ�
	SERIALIZE
		GameObject m_GameStartButton;
	SERIALIZE
		GameObject m_ConfigButton;
	SERIALIZE
		GameObject m_GameEndButton;
	
	//�ړ����̃I�u�W�F�N�g
	funifuni::TitleMoveButton m_MoveObject;
	funifuni::TitleMoveButton m_NowObject;

	//�{�^�����i�[����R���e�i
	std::vector<funifuni::TitleMoveButton> m_Buttons;
	float m_button_x;
	float m_button_y;

	//��ԑJ�ڕϐ�
	TitleButtonRoll m_button_state;

	//false equal left,true equal right
	bool is_arrow;

	bool is_next;

	SERIALIZE
		GameObject m_Canvas;
};

