
#pragma once
#include "main.h"


class EventWeaponEndurance :public IDllScriptComponent {
public:
	void Update();

private:
	//�����o�ϐ�
	SERIALIZE GameObject mTarget;
	SERIALIZE GameObject mOutput;

	SERIALIZE bool mIsEnd;

};