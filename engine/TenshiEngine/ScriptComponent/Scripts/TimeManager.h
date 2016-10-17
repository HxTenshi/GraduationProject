
#pragma once
#include "main.h"


class TimeManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnSlow();
	bool IsSlow();
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ƒƒ“ƒo•Ï”
	float isSlow_;
	float scaleTime_;
	SERIALIZE
	float slowRate_;
};