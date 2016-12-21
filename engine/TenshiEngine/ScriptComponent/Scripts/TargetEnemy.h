
#pragma once
#include "main.h"


class TargetEnemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetTargetEnemy(GameObject enemy);
	GameObject GetTargetEnemy();

private:

	void MarkarUpdate();
	void BarUpdate();
	//ÉÅÉìÉoïœêî
	GameObject m_TargetEnemy;

	SERIALIZE GameObject m_Camera;
	SERIALIZE GameObject m_EnemyHP_Bar;
	SERIALIZE GameObject m_EnemyHP_BarFrame;
	SERIALIZE GameObject m_EnemyTargetMarkar;
};