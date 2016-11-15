
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//�q�����ǂ���
	bool child = true;

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
};