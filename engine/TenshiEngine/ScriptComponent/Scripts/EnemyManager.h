
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//子分かどうか
	bool child = true;

	//攻撃するか
	bool attack = false;

	//次の攻撃までの時間をカウントするかどうか
	bool nextAttackTimeCountFlag = false;

	//今のアクションモード(TRACKING or BATTLE)のID
	ACTIONMODE nowActionMode = ACTIONMODE::TRACKINGMODE;
	
	//捜索中のパラメータ
	TrackingModeParameter trackingModeParameter;

	//戦闘中のパラメータ
	BattleModeParameter battleModeParameter;
};

struct EnemyOne {
	//個々の敵のgameObject
	GameObject enemyGameObject;

	//個々の敵のパラメータ
	EnemyParameter enemyParameter;
};

struct EnemyTeamParameter {
	//チームの番号
	int teamID = 0;

	//チームの人数
	int teamCount = 0;

	//誰が攻撃するか
	int whoAttack = 0;

	//次の攻撃までの時間
	float nextAttackTime = 0;

	//次の攻撃までの時間をカウントする
	float nextAttackTimeCount = 0;

	//攻撃が一週したか
	bool everyoneAttacked = false;

	//見つけているか
	bool discoveryPlayer = false;

	//見失ったか
	bool lostPlayer = false;

	//親が生きているかどうか
	bool parentAlive = true;
};

struct EnemyTeam {
	//個々の敵のgameObjectとパラメータをまとめたものがチーム分入ってるもの
	std::vector<EnemyOne> enemyOneVec;

	//チームのパラメータ
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
	//メンバ変数
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

	//Enemyの入れ物へGameObjectを入れる
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