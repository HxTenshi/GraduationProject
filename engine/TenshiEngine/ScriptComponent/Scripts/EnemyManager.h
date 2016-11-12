
#pragma once
#include "main.h"


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
	GameObject m_EnemyTeam;

	/// <summary>
	///Enemyの入れ物へGameObjectを入れる
	/// </summary>
	void EnemyTeamIntoEnemyContainer(GameObject g);

	std::vector<std::vector<GameObject>> m_EnemyContainer;
};