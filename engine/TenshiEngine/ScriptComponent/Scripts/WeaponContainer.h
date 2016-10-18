
#pragma once
#include "main.h"
# include "GetWeapon.h"

class WeaponContainer :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	//æ“¾Ï‚İ•Ší‚Ìæ“¾
	//GameObject GetWeapon();
	//•Ší‚Ìæ“¾
	void AddWeapon(GameObject target);
	void Release(GameObject weapon);
private:
	void GetChildren();
private:
	//ƒƒ“ƒo•Ï”
	std::list<GameObject> mWeapons;
	SERIALIZE
	GameObject mGetWeapon;
	GetWeapon *mWeapn;
	
};