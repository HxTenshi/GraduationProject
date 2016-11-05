
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
	NONE,
	CONFRONTACTION,
	APPROACHACTION,
	ATTACKDOWNACTION,
	JUMPATTACKACTION,
	GUARDACTION,
	BACKSTEPACTION,
	WINCEACTION,
	HITINGUARDACTION,
	ATTACKMONCKEYACTION
};

struct BattleModeParameter{	
	//バトル中のアクションのID
	BATTLEACTION battleActionID = BATTLEACTION::NONE;

	BATTLEACTION beforeBattleActionID = BATTLEACTION::NONE;
	
	//バトルモード中の思考時間を図るもの
	float count = 0.0f;

	//バトルモード中のアプローチとガード時にどちらに回転するか決める為
	bool rotateVecPlus = true;
};


enum ANIM_ID{
	ANIM_IDLE,
	ANIM_WALK_FORWARD,
	ANIM_BACKSTEP,
	ANIM_ATTACK_DOWN,
	ANIM_GUARD,
	ANIM_JUMPSRASH,
	ANIM_ATTACK_SIDE,
	ANIM_ATTACK_MONCKEY,
	ANIM_PROVOCATION,
	ANIM_RUSH,
	ANIM_SIDESTEPLEFT,
	ANIM_SIDESTEPRIGHT,
	ANIM_THRUST,
	ANIM_THRUSTRUN,
	ANIM_WINCE,
	ANIM_HITINGUARD
};

enum ACTIONMODE{
	TRACKINGMODE,
	BATTLEMODE
};

const float APPROACHTIME = 1.0f;
const float GUARDTIME = 1.0f;


class Sandbag :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	//void SetHitWall(bool hitWall);
	void Damage(int damage);

private:
	void AnimChange(int id, float lerpSpeed, bool roop = true);
	void AnimLerp();
	float GetNowAnimTime();

	void ChangeActionMode(ACTIONMODE nextActionMode);
	void ChangeBattleAction(BATTLEACTION nextBattleAction);
	void ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability);

	std::map<ACTIONMODE,std::function<void()>> actionModeInitilize;
	std::map<ACTIONMODE,std::function<void()>> actionModeUpdate;
	std::map<ACTIONMODE,std::function<void()>> actionModeFinalize;
	void TrackingModeInitilize();
	void TrackingModeUpdate();
	void TrackingModeFinalize();

	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	std::map<BATTLEACTION, std::function<void()>> battleActionInitilize;
	std::map<BATTLEACTION, std::function<void()>> battleActionUpdate;
	std::map<BATTLEACTION, std::function<void()>> battleActionFinalize;

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	void ApproachModeInitilize();
	void ApproachModeUpdate();
	void ApproachModeFinalize();

	void AttackModeInitilize();
	void AttackModeUpdate();
	void AttackModeFinalize();

	void JumpAttackModeInitilize();
	void JumpAttackModeUpdate();
	void JumpAttackModeFinalize();
	
	void GuardModeInitilize();
	void GuardModeUpdate();
	void GuardModeFinalize();

	void BackStepModeInitilize();
	void BackStepModeUpdate();
	void BackStepModeFinalize();

	void WinceModeInitilize();
	void WinceModeUpdate();
	void WinceModeFinalize();

	void HitInGuardModeInitilize();
	void HitInGuardModeUpdate();
	void HitInGuardModeFinalize();

	void AttackMonckeyModeInitilize();
	void AttackMonckeyModeUpdate();
	void AttackMonckeyModeFinalize();

	//メンバ変数
	SERIALIZE float trackingSpeed;
	SERIALIZE float trackingRange;
	SERIALIZE float onBattleRange;
	SERIALIZE float offBattleRange;
	SERIALIZE float trackingAngle;
	SERIALIZE float trackingRotateSpeed;
	SERIALIZE int hp;
	SERIALIZE GameObject player;
	SERIALIZE GameObject modelObject;
	SERIALIZE GameObject movePoints;
	SERIALIZE float aproachRotateSpeed;
	SERIALIZE float correctionRotateSpeed;
	SERIALIZE bool drawLog;

	//前方向
	XMVECTOR forward;
	//playerへのベクトル
	XMVECTOR playerVec;
	//自分の正面のベクトル自分からplayerへのベクトルの角度
	float view;
	//移動量
	XMVECTOR vec;

	//捜索時の移動ポイントの数値
	int moveCount;
	//捜索時の移動ポイントの方向
	bool moveCountUp;

	//アクションモードのID
	ACTIONMODE actionModeID;
	//バトル中のパラメーター
	BattleModeParameter battleModeParam;

	//アニメーションに必要なパラメーター
	AnimParameter animparam;

	//重力
	XMVECTOR mGravity;
};