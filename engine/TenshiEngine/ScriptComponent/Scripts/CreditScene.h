
#pragma once
#include "main.h"
#include <vector>


namespace Mossan {

	class ArrowControl {
	public:
		ArrowControl();
		ArrowControl(GameObject gameObject);
		void Update(float timer);
		void Enable();
		void Disable();
	private:
		GameObject gameObject;
		XMVECTOR m_initPos;
	};
};

class CreditScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void CreditOnDraw();
	void CreditNoDraw();
	void ChangeTexture();
	void CursorFunc(int num);
	bool IsCameraMoved();		//移動が完了したかどうか
	bool IsRightOver();
	bool IsLeftOver();

private:
	//メンバ変数
	SERIALIZE
		GameObject m_fader;
	SERIALIZE
		GameObject m_CameraObj;
	SERIALIZE
		XMVECTOR m_CameraDistance;
	SERIALIZE
		GameObject m_Itimaie;
	SERIALIZE
		GameObject m_weaponObj1;
	SERIALIZE
		GameObject m_weaponObj2;
	SERIALIZE
		GameObject m_weaponObj3;
	SERIALIZE
		GameObject m_weaponObj4;
	SERIALIZE
		GameObject m_weaponObj5;
	SERIALIZE
		GameObject m_weaponObj6;
	SERIALIZE
		GameObject m_weaponObj7;
	SERIALIZE
		GameObject m_weaponObj8;
	SERIALIZE
		GameObject m_weaponObj9;
	SERIALIZE
		GameObject m_weaponObj10;
	SERIALIZE
		GameObject m_weaponObj11;
	SERIALIZE
		GameObject m_weaponObj12;
	SERIALIZE
		GameObject m_weaponObj13;

	SERIALIZE
		GameObject arrowRight;
	SERIALIZE
		GameObject arrowLeft;
	Mossan::ArrowControl m_rightArrow;
	Mossan::ArrowControl m_leftArrow;
	float m_timer;

	std::vector<GameObject> m_objList;

	SERIALIZE
		int m_curNum;	//現在の選択番号
	int m_preNum;	//前回の選択番号
	float m_lerpTime;
};