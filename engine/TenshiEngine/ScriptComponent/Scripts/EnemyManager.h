
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//子分かどうか
	bool child = true;

	//攻撃するか
	bool attack = false;

	//攻撃が一週したか
	bool everyoneAttack = false;

	//次の攻撃までの時間をカウントするかどうか
	bool nextAttackTimeCountFlag = false;
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

	//攻撃が一週したか
	bool everyoneAttack = false;

	//次の攻撃までの時間をカウントする
	float nextAttackTimeCount = 0;

	//一斉攻撃までの時間をカウントするかどうか
	bool everyoneAttackCountFlag = false;

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
};