
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
	//�o�g�����̃A�N�V������ID
	BATTLEACTION battleActionID = BATTLEACTION::NONE;

	BATTLEACTION beforeBattleActionID = BATTLEACTION::NONE;
	
	//�o�g�����[�h���̎v�l���Ԃ�}�����
	float count = 0.0f;

	//�o�g�����[�h���̃A�v���[�`�ƃK�[�h���ɂǂ���ɉ�]���邩���߂��
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

	//�����o�ϐ�
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

	//�O����
	XMVECTOR forward;
	//player�ւ̃x�N�g��
	XMVECTOR playerVec;
	//�����̐��ʂ̃x�N�g����������player�ւ̃x�N�g���̊p�x
	float view;
	//�ړ���
	XMVECTOR vec;

	//�{�����̈ړ��|�C���g�̐��l
	int moveCount;
	//�{�����̈ړ��|�C���g�̕���
	bool moveCountUp;

	//�A�N�V�������[�h��ID
	ACTIONMODE actionModeID;
	//�o�g�����̃p�����[�^�[
	BattleModeParameter battleModeParam;

	//�A�j���[�V�����ɕK�v�ȃp�����[�^�[
	AnimParameter animparam;

	//�d��
	XMVECTOR mGravity;
};