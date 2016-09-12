
#pragma once
#include "main.h"


class ThunderLight :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(Actor* target)override;
	void OnCollideEnter(Actor* target)override;
	void OnCollideExit(Actor* target)override;

private:
	//メンバ変数
	XMFLOAT3 mColor;
	float mThounderPow;
};