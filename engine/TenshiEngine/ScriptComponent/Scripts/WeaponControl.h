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
	void IsEnemy(GameObject target, GameObject weapon);

private:
	//�����o�ϐ�
};