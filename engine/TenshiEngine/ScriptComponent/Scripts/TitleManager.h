
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
	//指定の位置をオーバーしたら
	bool ArrowOver();
	void Back();
	void Next();
	void SetMoveObject();
	void SetArrowPosition();
	void Select();
private:
	//メンバ変数
	SERIALIZE
		GameObject m_GameStartButton;
	SERIALIZE
		GameObject m_ConfigButton;
	SERIALIZE
		GameObject m_GameEndButton;
	
	//移動中のオブジェクト
	funifuni::TitleMoveButton m_MoveObject;
	funifuni::TitleMoveButton m_NowObject;

	//ボタンを格納するコンテナ
	std::vector<funifuni::TitleMoveButton> m_Buttons;
	float m_button_x;
	float m_button_y;

	//状態遷移変数
	TitleButtonRoll m_button_state;

	//false equal left,true equal right
	bool is_arrow;

	bool is_next;

	SERIALIZE
		GameObject m_Canvas;
};

