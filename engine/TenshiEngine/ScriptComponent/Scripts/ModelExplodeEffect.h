
#pragma once
#include "main.h"
#include <vector>
//physxÇ†ÇË
class ModelExplodeEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void Timer();
	void ExplodeFunc();
	void AutoDeleteTimer();
	void AutoDelete();
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject expModel1;
	SERIALIZE GameObject expModel2;
	SERIALIZE GameObject expModel3;
	SERIALIZE GameObject expModel4;
	SERIALIZE GameObject expModel5;

	std::vector<GameObject> objects;
	bool explode_flag;

	SERIALIZE float explode_time;
	float time;
	SERIALIZE float auto_delete_time;
	float count_auto_delete_time;
};