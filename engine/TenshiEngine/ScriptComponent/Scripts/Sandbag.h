
#pragma once
#include "main.h"

struct AnimParameter {
	int beforeAnimId = 0;
	int afterAnimId = 0;
	float nowLerpTime = 0;
	bool animLerpFlag = false;
	float lerpSpeed = 1.0f;
};


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

	void AnimChange(int id,float lerpSpeed);
	void AnimLerp();

	//ÉÅÉìÉoïœêî
	SERIALIZE float speed;
	SERIALIZE float trackingSpeed;
	SERIALIZE float trackingRange;
	SERIALIZE float trackingAngle;
	SERIALIZE float trackingRotateSpeed;
	SERIALIZE float rotateSpeed;
	SERIALIZE int hp;
	SERIALIZE GameObject player;
	SERIALIZE float concussionTime;
	SERIALIZE float jumpPower;
	SERIALIZE GameObject modelObject;
	
	AnimParameter animparam;
	XMVECTOR mGravity;
	bool changeVec,walkReturnFlag,damageFlag;
	float angle, maxAngle,subAngle,concussion;
	XMVECTOR vec;
};