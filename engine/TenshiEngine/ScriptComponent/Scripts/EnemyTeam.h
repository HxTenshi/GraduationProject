
#pragma once
#include "main.h"
#include "EnemyManager.h"

struct EnemyParameter {
	//攻撃するか
	bool attack = false;

	//攻撃が一週したか
	bool everyoneAttack = false;

	//次の攻撃までの時間をカウントするかどうか
	bool nextAttackTimeCountFlag = false;

	ENEMY_TYPE enemy_type = ENEMY_TYPE::PARENT;

	bool m_ArcharAttackStart = false;
};

struct EnemyOne {
	//個々の敵のgameObject
	GameObject enemyGameObject;

	//個々の敵のパラメータ
	EnemyParameter enemyParameter;
};

class EnemyTeam :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	virtual void TeamInitialize() {}
	virtual bool Alive() { return false; };
	virtual void DiscoveryOrLostPlayerSet() {};
	virtual void TeamUpdate() {};

	static EnemyTeam* GetEnemyTeam(GameObject target);
	void SetPlayer(GameObject gameObject) { m_Player = gameObject; };
private:
	//メンバ変数

protected:
	GameObject m_Player;
	
	std::vector<EnemyOne> teamMember;
	bool m_DiscoveryPlayer;
	bool wasAttacked;
	//誰が攻撃するか
	int whoAttack;

	//次の攻撃までの時間
	float nextAttackTime;

	//攻撃が一週したか
	bool everyoneAttack;

	//次の攻撃までの時間をカウントする
	float nextAttackTimeCount;

	//一斉攻撃までの時間をカウントするかどうか
	bool everyoneAttackCountFlag;
	//親が生きているかどうか
	bool parentAlive;

	int battlePosFirst;

	int archerCount = 0;
};