
#pragma once
#include "main.h"
#include "SoundManager.h"
#include "UniqueObject.h"
#include <map>
#include <vector>

class PauseMenuManager :public IDllScriptComponent{
public:
	struct Struct {
	public:
		GameObject m_texObj;
		XMVECTOR m_pivotPos;
		XMVECTOR m_pos1;
		XMVECTOR m_pos2;
		Struct(GameObject texObj,XMVECTOR pivotPos) {
			m_texObj = texObj;
			m_pivotPos = pivotPos;
		}
	};

	enum State {
		Close = 0,
		Open
	};

	//àÍñáäGóp
	enum Itimie{
		HowTo,
		Combo
	};

	enum Hint{
		one = 0,
		two,
		three,
		four,
		five
	};

public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void UpdateOpne();
	void UpdateClose();
	void OpenPauseMenu();
	void ClosePauseMenu();
	void MenuReAction(int num);
	void ChangeItimaie(Itimie type);
	void BackMenu();
	void BackToTitle();
	void OnHint();
	void LerpFunc(Struct* structObj,float lerpTime);
	void SE(SoundManager::SoundSE_ID::Enum key);

private:
	//ÉÅÉìÉoïœêî
	State m_state;
	Hint m_hint;
	bool m_isItimie;
	bool m_isHint;
	bool m_isHintLeftMove;
	bool m_isHintMoved;

	float m_lerpTimers[3];
	SERIALIZE
	GameObject m_Camvas;
	SERIALIZE
	GameObject m_texTips;
	SERIALIZE
	GameObject m_texPause;
	SERIALIZE
	GameObject m_texHowTo;
	SERIALIZE
	GameObject m_texCombo;
	SERIALIZE
	GameObject m_texToTitle;
	SERIALIZE
	GameObject m_texMigisita;
	SERIALIZE
	GameObject m_texItimie;
	SERIALIZE
	GameObject m_kakoi;
	SERIALIZE
	GameObject m_texBlack;
	SERIALIZE
	TextureAsset texHowTo;
	SERIALIZE
	TextureAsset texCombo;
	SERIALIZE
	TextureAsset texClose;
	SERIALIZE
	TextureAsset texBack;
	SERIALIZE
	float m_lerpSpeed;

	GameObject m_curHintTexObj;
	GameObject m_preHintTexObj;
	SERIALIZE
		GameObject texHintOne;
	SERIALIZE
		GameObject texHintTwo;
	SERIALIZE
		GameObject texHintThree;
	SERIALIZE
		GameObject texHintFour;
	SERIALIZE
		GameObject texHintFive;
	SERIALIZE
		GameObject rightArrow;
	SERIALIZE
		GameObject leftArrow;
	XMVECTOR rightArrowInitPos;
	XMVECTOR leftArrowInitPos;
	float  m_arrowTimer;
	std::vector<GameObject> m_texHintList;

	SERIALIZE GameObject m_overray_exit_option;

	int m_num;
	float m_stickInterval;


	std::map<int, std::vector<Struct*>> m_objMap;
};
