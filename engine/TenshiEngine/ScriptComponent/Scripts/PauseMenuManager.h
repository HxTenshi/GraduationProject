
#pragma once
#include "main.h"
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
	void LerpFunc(Struct* structObj,float lerpTime);

private:
	//ÉÅÉìÉoïœêî
	State m_state;
	bool m_isItimie;
	float m_lerpTimers[3];
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
	int m_num;
	std::map<int, std::vector<Struct*>> m_objMap;
};