
#pragma once
#include "main.h"
#include <vector>

class WeaponUIMemory :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	XMFLOAT4 GetMemColor(int n);
	void FillAlpha();
	void WeaponAttackParamMemSet(int atk);
private:
	//ƒƒ“ƒo•Ï”
	GameObject m_GageMem[15];
};