
#pragma once
#include "main.h"


class GetWeapon :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	struct MinVect {
		enum ENum {
			left,
			right
		};
	};

	GameObject GetMinWeapon();
	GameObject GetPointMinWeapon(GameObject currentTarget, MinVect::ENum minVect);

private:
	//ƒƒ“ƒo•Ï”
	std::list<GameObject> m_WeaponList;
	SERIALIZE
	GameObject m_Camera;
};