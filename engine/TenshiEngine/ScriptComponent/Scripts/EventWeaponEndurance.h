
#pragma once
#include "main.h"


class EventWeaponEndurance :public IDllScriptComponent {
public:
	void Update();

private:
	//メンバ変数
	SERIALIZE GameObject mTarget;
	SERIALIZE GameObject mOutput;

	SERIALIZE bool mIsEnd;

};