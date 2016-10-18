
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

	//取得済み武器の取得
	//GameObject GetWeapon();
	//武器の取得
	void AddWeapon(GameObject target);
	void Release(GameObject weapon);
private:
	void GetChildren();
private:
	//メンバ変数
	std::list<GameObject> mWeapons;
	SERIALIZE
	GameObject mGetWeapon;
	GetWeapon *mWeapn;
	
};