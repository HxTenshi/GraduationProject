
#pragma once
#include "main.h"


class ModelEffectController :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void UpperFunc();
	bool OverCheck();
	void AutoDelete();
private:
	SERIALIZE GameObject ModelEff;

	SERIALIZE bool isUpper;
	SERIALIZE float up;
	SERIALIZE float start_time;
	SERIALIZE float speed;
	float time;

	SERIALIZE float auto_delete_time;
	float delete_time;
};