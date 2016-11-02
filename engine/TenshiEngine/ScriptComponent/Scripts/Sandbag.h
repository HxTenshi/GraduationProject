
#pragma once
#include "main.h"

struct AnimParameter {
	int beforeAnimId = 0;
	int afterAnimId = 0;
	int nowAnimId = 0;
	float nowLerpTime = 0;
	bool animLerpFlag = false;
	float lerpSpeed = 1.0f;
	bool beforeAnimLoop = true;
	bool afterAnimLoop = true;
};

enum BATTLEACTION{
	ATTACK,
	GUARD,
	ESCAPE,
	TRACKING,
	NONE
};

enum ANIM_ID{
	ANIM_IDLE,
	ANIM_WALK_FORWARD,
	ANIM_BACKSTEP,
	ANIM_ATTACK_LONGITUDINAL,
	ANIM_GUARD,
	ANIM_ATTACK_SIDE,
	ANIM_JUMPSRASH,
	ANIM_MONCKEYATTACK,
	ANIM_PROVOCATION,
	ANIM_RUSH,
	ANIM_SIDESTEPLEFT,
	ANIM_SIDESTEPRIGHT,
	ANIM_THRUST,
	ANIM_THRUSTRUN,
};

struct BattleParameter{
	BATTLEACTION battleAction = BATTLEACTION::TRACKING;
	float actionDecideTime = 0.0f;
	int beforeActionDecideTime = 0;
	float guardTimeCount = 0;
};

enum ActionMode{
	TRACKINGMODE,
	BATTLEMODE
};

const float attackAnimTotalTime = 26.9f;
const float backStepAnimTotalTime = 39.9f;


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
	void AnimChange(int id, float lerpSpeed, bool roop = true);
	void AnimLerp();
	float GetNowAnimTime();

	//ActionMode
	void TrackingMode();
	void BattleMode();

	//ÉÅÉìÉoïœêî
	SERIALIZE float speed;
	SERIALIZE float trackingSpeed;
	SERIALIZE float trackingRange;
	SERIALIZE float battleRange;
	SERIALIZE float trackingAngle;
	SERIALIZE float trackingRotateSpeed;
	SERIALIZE float rotateSpeed;
	SERIALIZE int hp;
	SERIALIZE GameObject player;
	SERIALIZE float concussionTime;
	SERIALIZE float jumpPower;
	SERIALIZE GameObject modelObject;
	SERIALIZE GameObject movePoints;

	int moveCount;
	bool moveCountUp;
	ActionMode am;
	BattleParameter bp;
	AnimParameter animparam;
	XMVECTOR mGravity;
	bool changeVec,walkReturnFlag,damageFlag;
	float angle, maxAngle,subAngle,concussion;
	XMVECTOR vec;
};