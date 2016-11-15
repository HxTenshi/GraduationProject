
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//子分かどうか
	bool child = true;

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

	//Enemyの入れ物へGameObjectを入れる
	void EnemysIntoEnemyContainer();

	//
	std::vector<EnemyTeam> m_EnemyContainer;

	void SetActionMode(EnemyOne* enemyOne,ACTIONMODE actionMode) {
		auto jScript = enemyOne->enemyGameObject->GetScript<Enemy>();
		if (!jScript)return;
		enemyOne->enemyParameter.nowActionMode = actionMode;
		jScript->SetActionMode(actionMode);
	}

	void SetActionModeAndTrackingAction(EnemyOne* enemyOne, ACTIONMODE actionMode, TRACKINGACTION::Enum trackingAction) {
		auto jScript = enemyOne->enemyGameObject->GetScript<Enemy>();
		if (!jScript)return;
		enemyOne->enemyParameter.nowActionMode = actionMode;
		enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID = enemyOne->enemyParameter.trackingModeParameter.trackingActionID;
		enemyOne->enemyParameter.trackingModeParameter.trackingActionID = trackingAction;
		jScript->SetActionModeAndTrackingAction(actionMode,trackingAction);
	}

	void SetActionModeAndBattleAction(EnemyOne* enemyOne, ACTIONMODE actionMode, BATTLEACTION::Enum battleAction) {
		auto jScript = enemyOne->enemyGameObject->GetScript<Enemy>();
		if (!jScript)return;
		enemyOne->enemyParameter.nowActionMode = actionMode;
		enemyOne->enemyParameter.battleModeParameter.beforeBattleActionID = enemyOne->enemyParameter.battleModeParameter.battleActionID;
		enemyOne->enemyParameter.battleModeParameter.battleActionID = battleAction;
		jScript->SetActionModeAndBattleAction(actionMode, battleAction);
	}

	void SetTrackingAction(EnemyOne* enemyOne, TRACKINGACTION::Enum trackingAction) {
		auto jScript = enemyOne->enemyGameObject->GetScript<Enemy>();
		if (!jScript)return;
		enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID = enemyOne->enemyParameter.trackingModeParameter.beforetrackingActionID;
		enemyOne->enemyParameter.trackingModeParameter.trackingActionID = trackingAction;
		jScript->SetTrackingAction(trackingAction);
	}

	void SetBattleAction(EnemyOne* enemyOne, BATTLEACTION::Enum battleAction) {
		auto jScript = enemyOne->enemyGameObject->GetScript<Enemy>();
		if (!jScript)return;
		enemyOne->enemyParameter.battleModeParameter.beforeBattleActionID = enemyOne->enemyParameter.battleModeParameter.battleActionID;
		enemyOne->enemyParameter.battleModeParameter.battleActionID = battleAction;
		jScript->SetBattleAction(battleAction);
	}
};