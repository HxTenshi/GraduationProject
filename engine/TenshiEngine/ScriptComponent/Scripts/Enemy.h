
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
	bool naviMeshFlag = false;

	//�e�������Ă��邩�ǂ���
	bool parentAlive = true;
};

struct BATTLEACTION {
	enum Enum {
	NONE,
	CONFRONTACTION,
	APPROACHACTION,
	ATTACK1ACTION,
	ATTACK2ACTION,
	ATTACK3ACTION,
	ATTACK4ACTION,
	ATTACK5ACTION,
	SHOTACTION,
	GUARDACTION,
	PROVOCATION,
	BACKSTEPACTION,
	WINCEACTION,
	UPPERDOWNACTION,
	BEATDOWNACTION,
	DOWNACTION,
	HITINGUARDACTION,
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

enum ACTIONMODE{
	TRACKINGMODE,
	BATTLEMODE
};

struct EnemyAllParamter{
	TrackingModeParameter trackingModeParameter;
	BattleModeParameter battleModeParameter;
	ACTIONMODE actionMode;
	float maxHp;
	float hp;
	float damage;
};

enum ENEMY_TYPE {
	PARENT,
	CHILD,
	PARENT_ARCHER,
	CHILD_ARCHER,
};


enum COL_TYPE {
	NORMAL,
	ROTATE,
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
	virtual void ChildInitialize() {}
	virtual void SoloAction() {}
	virtual ENEMY_TYPE GetEnemyType() { return ENEMY_TYPE::CHILD; };
	virtual float GetOnBattleRange() { return 0.0f; };
	virtual void Attack(GameObject player,COL_TYPE colType) {};
	virtual bool Damage(float damage_, BATTLEACTION::Enum winceType_,XMVECTOR accelPower_) { return false; };
	virtual void ChildFinalize() {}
	virtual bool DiscoveryPlayer() { return false; };
	virtual bool LostPlayer() { return false;};
	bool IsEnd() { return m_Isend; }
	bool GetWasAttacked() {
		auto ret = m_WasAttacked;
		m_WasAttacked = false;
		return ret;
	}

	static Enemy* GetEnemy(GameObject target);

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
		eap.hp = m_Hp;
		eap.maxHp = m_MaxHp;
		eap.damage = m_Damage;
		return eap;
	}
	void SetBattlePosition(XMVECTOR battlePosition_) { m_BattleModeParam.battlePosition = battlePosition_; }
	void AnimChange(int id, float lerpSpeed, bool roop = true, bool forcingChange = false);
	float GetNowAnimTime();

private:
	void AnimLerp();
	
protected:
	GameObject ModelObject;
protected:
	void LookPosition(XMVECTOR position_, float rotateSpeed, bool zReset = false);

	std::map<ACTIONMODE,std::function<void()>> actionModeInitilize;
	std::map<ACTIONMODE,std::function<void()>> actionModeUpdate;
	std::map<ACTIONMODE,std::function<void()>> actionModeFinalize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionInitilize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionUpdate;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionFinalize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionInitilize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionUpdate;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionFinalize;

	GameObject m_Player;

	//�O����
	XMVECTOR m_Forward;
	//m_Player�ւ̃x�N�g��
	XMVECTOR m_PlayerVec;
	//�����̐��ʂ̃x�N�g����������m_Player�ւ̃x�N�g���̊p�x
	float m_View;
	//�ړ���
	XMVECTOR m_Vec;

	XMVECTOR m_AccelVec; 
	XMVECTOR m_Accel;
	//�󂯂�_���[�W��ۑ��������
	float m_Damage;

	//�{�����̈ړ��|�C���g�̐��l
	int m_MoveCount;
	//�{�����̈ړ��|�C���g�̕���
	bool m_MoveCountUp;

	bool m_Isend;

	float m_Hp;
	float m_MaxHp;

	std::map<int,GameObject> m_MovePointsVec;

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

	bool m_WasAttacked;
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

	void ChangeActionMode(ACTIONMODE nextActionMode);
	void ChangeActionAndTrackingAction(ACTIONMODE nextActionMode, TRACKINGACTION::Enum nextTrackingAction);
	void ChangeActionAndBattleAction(ACTIONMODE nextActionMode, BATTLEACTION::Enum nextBattleAction);
	void ChangeTrackingAction(TRACKINGACTION::Enum nextTrackingAction);
	void ChangeBattleAction(BATTLEACTION::Enum nextBattleAction);
};