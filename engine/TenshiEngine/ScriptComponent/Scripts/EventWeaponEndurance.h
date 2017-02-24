
#pragma once
#include "main.h"


class EventWeaponEndurance :public IDllScriptComponent {
public:
	void Update();

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject mTarget;
	SERIALIZE GameObject mOutput;

	SERIALIZE bool mIsEnd;

};