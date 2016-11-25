
#pragma once
#include "main.h"
#include "Enemy.h"

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

	std::vector<GameObject> m_EnemyTeamVector;
};