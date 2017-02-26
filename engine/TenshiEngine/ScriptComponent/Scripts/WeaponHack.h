
#pragma once
#include "main.h"


class WeaponHack :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

private:
	SERIALIZE GameObject mWeapon;
};