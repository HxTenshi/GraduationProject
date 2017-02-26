
#pragma once
#include "main.h"


class CheackWeaponHP :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

private:
	SERIALIZE GameObject mOutputEvent;
	SERIALIZE GameObject mWeapon;
};