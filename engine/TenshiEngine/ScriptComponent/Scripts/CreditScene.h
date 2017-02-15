
#pragma once
#include "main.h"
#include <vector>


class CreditScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
		GameObject m_CameraObj;
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

	std::vector<GameObject> m_objList;
	int m_cursorNum;
};