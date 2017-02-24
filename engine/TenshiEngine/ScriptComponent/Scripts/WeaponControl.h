//Weapon���G�ɓ��������Ƃ��Ƃ��ɌĂԊ֐����`���Ă����B

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

	//Weapon��Bigin�ŌĂ�ł��炤
	void HitActor(GameObject target, GameObject weapon);
	bool SearchEnemyBone(GameObject target, GameObject weapon,std::string name);

	bool IsHitHead(GameObject target);
	bool IsHit();
	void Hit();
	void DeleteHitPoint();

	void HitStage(GameObject target, GameObject weapon, weak_ptr<PhysXComponent> physices);

private:
	//�����o�ϐ�
	bool isHit;
	float mTime;
	SERIALIZE
	float mHitTime;
};