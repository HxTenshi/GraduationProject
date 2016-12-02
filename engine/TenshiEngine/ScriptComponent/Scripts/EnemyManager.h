
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
	//ƒƒ“ƒo•Ï”
	SERIALIZE
	GameObject m_Enemys;
	SERIALIZE
	bool m_DrawFlag;
	SERIALIZE
	GameObject m_Player;

	//Enemy‚Ì“ü‚ê•¨‚ÖGameObject‚ğ“ü‚ê‚é
	void EnemysIntoEnemyContainer();

	//Enemy‚Ì“ü‚ê•¨
	std::vector<GameObject> m_EnemyTeamVector;
};