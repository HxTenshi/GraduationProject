
#pragma once
#include "main.h"
#include <vector>

class EnemyMinotaurAngerEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void Anger();
private:
	//ÉÅÉìÉoïœêî
	std::vector<GameObject> effect;
	SERIALIZE GameObject obj1;
	SERIALIZE GameObject obj2;
	SERIALIZE GameObject obj3;
	SERIALIZE GameObject obj4;
	SERIALIZE GameObject obj5;
};