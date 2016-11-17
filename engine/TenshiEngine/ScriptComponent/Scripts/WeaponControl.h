//Weaponが敵に当たったときとかに呼ぶ関数を定義しておく。

#pragma once
#include "main.h"


class WeaponControl :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	//WeaponのBiginで呼んでもらう
	void HitActor(GameObject target, GameObject weapon);
	void SearchEnemyBone(GameObject target, GameObject weapon,std::string name);

	bool IsHitHead(GameObject target);
	bool IsHit();
	void DeleteHitPoint();

private:
	//メンバ変数
	bool isHit;
	float mTime;
	SERIALIZE
	float mHitTime;
};