
#pragma once
#include "main.h"


class EnemyArrow :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void SetEnemy(GameObject g) { m_Enemy = g; }
	void SetVec(XMVECTOR v) { m_Vec = v; }
private:
	//ƒƒ“ƒo•Ï”
	GameObject m_Enemy;
	XMVECTOR m_Vec;
	float count;
	bool m_Stop;
	SERIALIZE float speed;
};