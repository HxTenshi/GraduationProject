
#pragma once
#include "main.h"


class Sandbag :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void SetHitWall(bool hitWall);

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE float speed;
	XMVECTOR mGravity;
	bool changeVec;
	float angle;
};