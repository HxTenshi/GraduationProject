
#pragma once
#include "main.h"
#include <vector>
#include "h_standard.h"
#include "SoundManager.h"

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
	public:
		GameObject obj;
	};

};

class TitleManager :public IDllScriptComponent {
public:
	enum State {
		state1 = 0,
		state2
	};

	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void InitPosition(bool arrow);
	//�w��̈ʒu���I�[�o�[������
	bool ArrowOver();
	void Back();
	void Next();
	void SetMoveObject();
	void SetArrowPosition();
	void Select();
	void Select2(int num);
	void OnBGM();
	void OnSE(SoundManager::SoundSE_ID::Enum seID);
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

	State m_state;
	int selectNum;

	//false equal left,true equal right
	bool is_arrow;

	bool is_next;

	bool iup_flag;

	bool is_bgm;

	SERIALIZE
		GameObject m_canvas1;
	SERIALIZE
		GameObject m_canvas2;
	SERIALIZE
		GameObject m_kakoi;
	SERIALIZE
		std::string gameStartScenePass;
	SERIALIZE
		std::string turorialScenePass;
	SERIALIZE
		std::string configScenePass;
	SERIALIZE
		GameObject m_fader;
};

