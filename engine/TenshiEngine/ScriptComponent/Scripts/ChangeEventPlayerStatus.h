
#pragma once
#include "main.h"


class ChangeEventPlayerStatus :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

	void OnSetSP();

	void LockWeapon();

private:
	SERIALIZE float mSP;
	SERIALIZE bool mSPUpdate;
	SERIALIZE bool mLock;
};