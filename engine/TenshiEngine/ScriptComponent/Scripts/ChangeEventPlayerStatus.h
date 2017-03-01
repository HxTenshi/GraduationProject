
#pragma once
#include "main.h"


class ChangeEventPlayerStatus :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

	void OnSetSP();

	void LockWeapon();
	void ThrowWeapon();

private:
	SERIALIZE float mSP;
	SERIALIZE bool mSPUpdate;
	SERIALIZE bool mLock;
	SERIALIZE bool mThrow;
};