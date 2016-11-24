
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
	TRACKINGACTION::Enum id = TRACKINGACTION::NONE;

	//�O�̃A�N�V������ID
	TRACKINGACTION::Enum beforeId = TRACKINGACTION::NONE;
	
	//�i�r���b�V�����g�����ǂ���
	bool naviMeshFlag = true;

	//�e�������Ă��邩�ǂ���
	bool parentAlive = true;
};

struct BATTLEACTION {
	enum Enum {
	NONE,
	CONFRONTACTION,
	APPROACHACTION,
	ATTACKDOWNACTION,
	JUMPATTACKACTION,
	GUARDACTION,
	PROVOCATION,
	BACKSTEPACTION,
	WINCEACTION,
	HITINGUARDACTION,
	ATTACKMONCKEYACTION,
	DEADACTION
	};
};

struct BattleModeParameter{	
	//���̃A�N�V������ID
	BATTLEACTION::Enum id = BATTLEACTION::NONE;

	//�O�̃A�N�V������ID
	BATTLEACTION::Enum beforeId = BATTLEACTION::NONE;
	
	//�o�g�����[�h���̎v�l���Ԃ�}�����
	float count = 0.0f;

	//���낤�낷�鎞��
	float decideAprochTime = 0;

	//�����A�N�V���������񑱂������J�E���g
	int sameActionCount = 0;

	//�����A�N�V�������A���ōs���Ă���ꍇ���̃A�N�V�����͈�ԍŏ����H
	bool firstInSameAction = true;

	//�o�g�����[�h���̃A�v���[�`�ƃK�[�h���ɂǂ���ɉ�]���邩���߂��
	bool rotateVecPlus = true;

	//�A�N�V�������I��������ǂ���
	bool actionFinish = false;

	//�|�W�V�����ɂ�����
	bool arrival = false;

	//�U���ɑJ�ڂł���A�N�V�������ǂ���
	bool canChangeAttackAction = false;

	//�킢���s���ꏊ
	XMVECTOR battlePosition = XMVectorSet(0, 0, 0, 0);
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

struct EnemyAllParamter{
	TrackingModeParameter trackingModeParameter;
	BattleModeParameter battleModeParameter;
	ACTIONMODE actionMode;
};

template<class T>
const T& clamp(const T& v, const T& min, const T& max) {
	if (v <= min)return min;
	else if (v >= max)return max;
	return v;
}


class Enemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	virtual void FunctionSet() = 0;
	virtual bool GetChildFlag() = 0;
	virtual float GetOnBattleRange() = 0;
	virtual void Attack(GameObject player) = 0;
	virtual void Damage(float damage_) = 0;
	virtual bool DiscoveryPlayer() = 0;
	virtual bool LostPlayer() = 0;
	void SetParentAlive(bool flag) { m_TrackingModeParam.parentAlive = flag; }
	EnemyAllParamter GetEnemyAllParameter(bool reset) {
		EnemyAllParamter eap;
		auto bmp = m_BattleModeParam;
		if (reset) {
			m_BattleModeParam.actionFinish = false;
			m_BattleModeParam.arrival = false;
		}
		eap.trackingModeParameter = m_TrackingModeParam;
		eap.battleModeParameter = bmp;
		eap.actionMode = m_ActionModeID;
		return eap;
	}
	void SetBattlePosition(XMVECTOR battlePosition_) { m_BattleModeParam.battlePosition = battlePosition_; }
	void SetPlayer(GameObject player) { m_Player = player; }
	void AnimChange(int id, float lerpSpeed, bool roop = true, bool forcingChange = false);
	float GetNowAnimTime();

private:
	void AnimLerp();

	void ChangeActionMode(ACTIONMODE nextActionMode);
	void ChangeActionAndTrackingAction(ACTIONMODE nextActionMode, TRACKINGACTION::Enum nextTrackingAction);
	void ChangeActionAndBattleAction(ACTIONMODE nextActionMode, BATTLEACTION::Enum nextBattleAction);
	void ChangeTrackingAction(TRACKINGACTION::Enum nextTrackingAction);
	void ChangeBattleAction(BATTLEACTION::Enum nextBattleAction);
	
protected:
	GameObject ModelObject;
protected:

	std::map<ACTIONMODE,std::function<void()>> actionModeInitilize;
	std::map<ACTIONMODE,std::function<void()>> actionModeUpdate;
	std::map<ACTIONMODE,std::function<void()>> actionModeFinalize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionInitilize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionUpdate;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionFinalize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionInitilize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionUpdate;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionFinalize;
	GameObject m_gameObject = gameObject;

	GameObject m_Player;

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
	void SetActionID(ACTIONMODE actionMode) {
		m_ActionModeID = actionMode;
	}
	void SetTrackingID(TRACKINGACTION::Enum trackingAction) {
		m_TrackingModeParam.id = trackingAction;
	}
	void SetBattleID(BATTLEACTION::Enum battleAction) {
		m_BattleModeParam.id = battleAction;
	}

	void SetActionMode(ACTIONMODE actionMode) {
		ChangeActionMode(actionMode);
	}

	void SetActionModeAndTrackingAction(ACTIONMODE actionMode, TRACKINGACTION::Enum trackingAction) {
		ChangeActionAndTrackingAction(actionMode, trackingAction);
	}

	void SetActionModeAndBattleAction(ACTIONMODE actionMode, BATTLEACTION::Enum battleAction) {
		ChangeActionAndBattleAction(actionMode, battleAction);
	}

	void SetTrackingAction(TRACKINGACTION::Enum trackingAction) {
		ChangeTrackingAction(trackingAction);
	}

	void SetBattleAction(BATTLEACTION::Enum battleAction) {
		ChangeBattleAction(battleAction);
	}


};