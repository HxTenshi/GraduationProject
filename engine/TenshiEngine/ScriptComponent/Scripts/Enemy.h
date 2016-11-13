
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

struct TRACKINGACTION {
	enum Enum {
		NONE,
		PARENTTRACKING,
		CHILDTRACKING,
	};
};

struct TrackingModeParameter {
	//���̃A�N�V������ID
	TRACKINGACTION::Enum trackingActionID = TRACKINGACTION::NONE;

	//�O�̃A�N�V������ID
	TRACKINGACTION::Enum beforetrackingActionID = TRACKINGACTION::NONE;
	
	//�i�r���b�V�����g�����ǂ���
	bool naviMeshFlag = true;
};

struct BATTLEACTION {
	enum Enum {
	NONE,
	CONFRONTACTION,
	APPROACHACTION,
	ATTACKDOWNACTION,
	JUMPATTACKACTION,
	GUARDACTION,
	BACKSTEPACTION,
	WINCEACTION,
	HITINGUARDACTION,
	ATTACKMONCKEYACTION,
	DEADACTION
	};
};

struct BattleModeParameter{	
	//���̃A�N�V������ID
	BATTLEACTION::Enum battleActionID = BATTLEACTION::NONE;

	//�O�̃A�N�V������ID
	BATTLEACTION::Enum beforeBattleActionID = BATTLEACTION::NONE;
	
	//�o�g�����[�h���̎v�l���Ԃ�}�����
	float count = 0.0f;

	//�����A�N�V���������񑱂������J�E���g
	int sameActionCount = 0;

	//�����A�N�V�������A���ōs���Ă���ꍇ���̃A�N�V�����͈�ԍŏ����H
	bool firstInSameAction = true;

	//�o�g�����[�h���̃A�v���[�`�ƃK�[�h���ɂǂ���ɉ�]���邩���߂��
	bool rotateVecPlus = true;
};


enum ANIM_ID{
	ANIM_IDLE,
	ANIM_WALK_FORWARD,
	ANIM_BACKSTEP,
	ANIM_ATTACK_DOWN,
	ANIM_GUARD,
	ANIM_JUMPATTACK,
	ANIM_WINCE,
	ANIM_HITINGUARD,
	ANIM_ATTACK_MONCKEY,
	ANIM_PROVOCATION,
	ANIM_ATTACK_SIDE,
	ANIM_RUSH,
	ANIM_SIDESTEPLEFT,
	ANIM_SIDESTEPRIGHT,
	ANIM_THRUST,
	ANIM_THRUSTRUN,
};

enum ACTIONMODE{
	TRACKINGMODE,
	BATTLEMODE
};

const float APPROACHTIME = 1.0f;
const float GUARDTIME = 1.0f;


class Enemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void ManagerInitialize();
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void Damage(float damage_);
	void Attack(GameObject player);
	bool GetChildFlag() { return m_Child; }

private:
	void AnimChange(int id, float lerpSpeed, bool roop = true, bool forcingChange = false);
	void AnimLerp();
	float GetNowAnimTime();

	void ChangeActionMode(ACTIONMODE nextActionMode);
	void ChangeBattleAction(BATTLEACTION::Enum nextBattleAction);
	void ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability);

	std::map<ACTIONMODE,std::function<void()>> actionModeInitilize;
	std::map<ACTIONMODE,std::function<void()>> actionModeUpdate;
	std::map<ACTIONMODE,std::function<void()>> actionModeFinalize;

	void TrackingModeInitilize();
	void TrackingModeUpdate();
	void TrackingModeFinalize();

	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionInitilize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionUpdate;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionFinalize;

	void ParentTrackingModeInitilize();
	void ParentTrackingModeUpdate();
	void ParentTrackingModeFinalize();

	void ChildTrackingModeInitilize();
	void ChildTrackingModeUpdate();
	void ChildTrackingModeFinalize();

	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionInitilize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionUpdate;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionFinalize;

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	void ApproachModeInitilize();
	void ApproachModeUpdate();
	void ApproachModeFinalize();

	void AttackDownModeInitilize();
	void AttackDownModeUpdate();
	void AttackDownModeFinalize();

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

	//����͉���䖝���邩(m_HitInGuardMinCount�`m_HitInGuardMaxCount)
	int PatienceInThisTime;

	void AttackMonckeyModeInitilize();
	void AttackMonckeyModeUpdate();
	void AttackMonckeyModeFinalize();

	void DeadModeInitilize();
	void DeadModeUpdate();
	void DeadModeFinalize();

	//�����o�ϐ�
	SERIALIZE float m_TrackingSpeed;
	SERIALIZE float m_TrackingRange;
	SERIALIZE float m_OnBattleRange;
	SERIALIZE float m_OffBattleRange;
	SERIALIZE float m_TrackingAngle;
	SERIALIZE float m_TrackingRotateSpeed;
	SERIALIZE float m_Hp;
	SERIALIZE int m_AttackDamage;
	SERIALIZE GameObject m_Player;
	SERIALIZE GameObject m_MyWeapon;
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE GameObject m_MovePoints;
	SERIALIZE bool m_Child;
	SERIALIZE float m_AproachRotateSpeed;
	SERIALIZE float m_CorrectionRotateSpeed;
	SERIALIZE bool m_DrawLog;
	SERIALIZE int m_HitInGuardMinCount;
	SERIALIZE int m_HitInGuardMaxCount;
	SERIALIZE int m_AbsolutelyAvoidInHitAttackProbability;

	//�O����
	XMVECTOR m_Forward;
	//m_Player�ւ̃x�N�g��
	XMVECTOR m_PlayerVec;
	//�����̐��ʂ̃x�N�g����������m_Player�ւ̃x�N�g���̊p�x
	float m_View;
	//�ړ���
	XMVECTOR m_Vec;
	//�󂯂�_���[�W��ۑ��������
	float m_Damage;

	//�{�����̈ړ��|�C���g�̐��l
	int m_MoveCount;
	//�{�����̈ړ��|�C���g�̕���
	bool m_MoveCountUp;


	std::vector<GameObject> m_MovePointsVec;

	//�A�N�V�������[�h��ID
	ACTIONMODE m_ActionModeID;
	//�o�g�����̃p�����[�^�[
	BattleModeParameter m_BattleModeParam;
	//�{�����̃p�����[�^�[
	TrackingModeParameter m_TrackingModeParam;

	//�A�j���[�V�����ɕK�v�ȃp�����[�^�[
	AnimParameter m_Animparam;

	float m_ChildTranckingSpeed;
	//�d��
	XMVECTOR m_Gravity;

public:
	void ActionInitilize(TRACKINGACTION::Enum trackingAction);
	void ActionUpdate(TRACKINGACTION::Enum trackingAction);
	void ActionFinalize(TRACKINGACTION::Enum trackingAction);

	void ActionInitilize(BATTLEACTION::Enum battleAction);
	void ActionUpdate(BATTLEACTION::Enum battleAction);
	void ActionFinalize(BATTLEACTION::Enum battleAction);
};