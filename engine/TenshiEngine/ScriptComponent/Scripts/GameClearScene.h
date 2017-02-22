
#pragma once
#include "main.h"
#include <string>
#include <h_component.h>
#include <h_standard.h>

namespace Mossan {

	class GameClearTexObj {
	public:
		GameClearTexObj(GameObject gameObject);
		GameClearTexObj();
		void WaitSmall();	//テクスチャを小さくして維持
		void WaitBig();		//テクスチャを大きくして維持
		void Wipe();		//テクスチャを徐々に広げる
		void Small();		//テクスチャを徐々に小さくする
		void Enable();
		void Disable();
		std::string GetObjName();
		bool IsEnd();
	private:
		GameObject m_texObj;
		XMVECTOR m_initScale;
		bool m_isEnd;
	};

	class GameClearTextObj {
	public:
		GameClearTextObj(GameObject gameObject);
		GameClearTextObj();
		void UpdateScore();		//スコア用のアップデート
		void UpdateTime();		//タイム用のアップデート
		void ChangeText(std::string message);
		void SetScore(int num);
		int GetScore();
		bool IsEnd();
	private:
		GameObject m_textObj;	//テキストオブジェ
		float m_numberf;			//表示する数値(float)
		int	m_number;			//表示する数値
		int m_pivotNum;			//基準値
		bool m_isEnd;
	};
};

class GameClearScene :public IDllScriptComponent{
public:
	enum Phase {
		phase1 = 0,
		phase2,
		phase3,
		phase4
	};

	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void DoPhase1();
	void DoPhase2();
	void DoPhase3();
	void DoPhase4();


private:
	//メンバ変数
	SERIALIZE
		GameObject m_texClear;
	SERIALIZE
		GameObject m_texKill;
	SERIALIZE
		GameObject m_texTime;
	SERIALIZE
		GameObject m_textKill;
	SERIALIZE
		GameObject m_textTime;
	SERIALIZE
		GameObject m_fader;

	Mossan::GameClearTexObj m_texClearObj;
	Mossan::GameClearTexObj m_texKillObj;
	Mossan::GameClearTexObj m_texTimeObj;
	Mossan::GameClearTextObj m_textKillObj;
	Mossan::GameClearTextObj m_textTimeObj;

	Phase m_phase;
};