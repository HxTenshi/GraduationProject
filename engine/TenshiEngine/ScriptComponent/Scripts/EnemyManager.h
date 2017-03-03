
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

	static std::vector<GameObject> m_Enemys;
	static void EnemyPush(GameObject gameObject_);
	static void EnemyErase(GameObject gameObject_);
	static std::vector<GameObject> GetEnemy();
private:
};