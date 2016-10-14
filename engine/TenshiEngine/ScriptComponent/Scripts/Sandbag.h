
#pragma once
#include "main.h"


class Sandbag :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void SetHitWall(bool hitWall);
	void Damage(int damage);

private:
	void Walk();
	void WallHit();

	//�����o�ϐ�
	SERIALIZE float speed;
	SERIALIZE float rotateSpeed;
	SERIALIZE int hp;
	XMVECTOR mGravity;
	bool changeVec,walkReturnFlag,damageFlag;
	float angle, maxAngle,subAngle,concussion;
	XMVECTOR vec;
};