
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//�q�����ǂ���
	bool child = true;

	//�U�����邩
	bool attack = false;

	//���̍U���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	bool nextAttackTimeCountFlag = false;

	//���̃A�N�V�������[�h(TRACKING or BATTLE)��ID
	ACTIONMODE nowActionMode = ACTIONMODE::TRACKINGMODE;
	
	//�{�����̃p�����[�^
	TrackingModeParameter trackingModeParameter;

	//�퓬���̃p�����[�^
	BattleModeParameter battleModeParameter;
};

struct EnemyOne {
	//�X�̓G��gameObject
	GameObject enemyGameObject;

	//�X�̓G�̃p�����[�^
	EnemyParameter enemyParameter;
};

struct EnemyTeamParameter {
	//�`�[���̔ԍ�
	int teamID = 0;

	//�`�[���̐l��
	int teamCount = 0;

	//�N���U�����邩
	int whoAttack = 0;

	//���̍U���܂ł̎���
	float nextAttackTime = 0;

	//���̍U���܂ł̎��Ԃ��J�E���g����
	float nextAttackTimeCount = 0;

	//�U������T������
	bool everyoneAttacked = false;

	//�����Ă��邩
	bool discoveryPlayer = false;

	//����������
	bool lostPlayer = false;

	//�e�������Ă��邩�ǂ���
	bool parentAlive = true;
};

struct EnemyTeam {
	//�X�̓G��gameObject�ƃp�����[�^���܂Ƃ߂����̂��`�[���������Ă����
	std::vector<EnemyOne> enemyOneVec;

	//�`�[���̃p�����[�^
	EnemyTeamParameter enemyTeamParameter;
};

class EnemyManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//�����o�ϐ�
	SERIALIZE
	GameObject m_Enemys;
	SERIALIZE
	bool m_DrawFlag;
	SERIALIZE
	GameObject m_Player;
	SERIALIZE
	float m_NextAttackTimeMin;
	SERIALIZE
	float m_NextAttackTimeMax;

	//Enemy�̓��ꕨ��GameObject������
	void EnemysIntoEnemyContainer();

	//
	std::vector<EnemyTeam> m_EnemyContainer;

	void SetActionMode(EnemyOne* enemyOne, Enemy* jScript, ACTIONMODE actionMode) {
		enemyOne->enemyParameter.nowActionMode = actionMode;
		jScript->SetActionMode(actionMode);
	}

	void SetActionModeAndTrackingAction(EnemyOne* enemyOne,Enemy* jScript, ACTIONMODE actionMode, TRACKINGACTION::Enum trackingAction) {
		enemyOne->enemyParameter.nowActionMode = actionMode;
		enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID = enemyOne->enemyParameter.trackingModeParameter.trackingActionID;
		enemyOne->enemyParameter.trackingModeParameter.trackingActionID = trackingAction;
		jScript->SetActionModeAndTrackingAction(actionMode,trackingAction);
	}

	void SetActionModeAndBattleAction(EnemyOne* enemyOne, Enemy* jScript, ACTIONMODE actionMode, BATTLEACTION::Enum battleAction) {
		enemyOne->enemyParameter.nowActionMode = actionMode;
		enemyOne->enemyParameter.battleModeParameter.beforeBattleActionID = enemyOne->enemyParameter.battleModeParameter.battleActionID;
		enemyOne->enemyParameter.battleModeParameter.battleActionID = battleAction;
		jScript->SetActionModeAndBattleAction(actionMode, battleAction);
	}

	void SetTrackingAction(EnemyOne* enemyOne, Enemy* jScript, TRACKINGACTION::Enum trackingAction) {
		enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID = enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID;
		enemyOne->enemyParameter.trackingModeParameter.trackingActionID = trackingAction;
		jScript->SetTrackingAction(trackingAction);
	}

	void SetBattleAction(EnemyOne* enemyOne, Enemy* jScript, BATTLEACTION::Enum battleAction) {
		enemyOne->enemyParameter.battleModeParameter.beforeBattleActionID = enemyOne->enemyParameter.battleModeParameter.battleActionID;
		enemyOne->enemyParameter.battleModeParameter.battleActionID = battleAction;
		jScript->SetBattleAction(battleAction);
	}

	BATTLEACTION::Enum ChangeBattleAction(
		int guardProbability, int approachProbability,
		int backstepProbability, int attackProbability,
		int jumpAttackProbability,int provocationProbability
		) {
		//if (XMVector3Length(m_PlayerVec).x > m_OffBattleRange && m_BattleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION) {
		//	ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
		//	return;
		//}
		int totalProbability = 
			guardProbability + approachProbability +
			backstepProbability + attackProbability +
			jumpAttackProbability + provocationProbability;

		int randam = rand() % totalProbability;

		int guardProbability_, approachProbability_,
			backstepProbability_, attackProbability_,
			jumpAttackProbability_, provocationProbability_;

		guardProbability_ = totalProbability - guardProbability;
		approachProbability_ = guardProbability_ - approachProbability;
		backstepProbability_ = approachProbability_ - backstepProbability;
		attackProbability_ = backstepProbability_ - attackProbability;
		jumpAttackProbability_ = attackProbability_ - jumpAttackProbability;
		provocationProbability_ = jumpAttackProbability_ - provocationProbability;

		if (randam > guardProbability_) {
			return BATTLEACTION::GUARDACTION;
		}
		else if (randam > approachProbability_) {
			return BATTLEACTION::APPROACHACTION;
		}
		else if (randam > backstepProbability_) {
			return BATTLEACTION::BACKSTEPACTION;
		}
		else if (randam > attackProbability_) {
			return BATTLEACTION::ATTACKDOWNACTION;
		}
		else if (randam > jumpAttackProbability_) {
			return BATTLEACTION::JUMPATTACKACTION;
		}
		else if (randam > provocationProbability_) {
			return BATTLEACTION::PROVOCATION;
		}
		return BATTLEACTION::CONFRONTACTION;
	}
};