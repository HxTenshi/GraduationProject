
#pragma once
#include "main.h"


class TutorialUI :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;

private:
	//UI—p
	SERIALIZE GameObject mBattleUIs;
	SERIALIZE GameObject mWeakAttack;
	SERIALIZE GameObject mStrongAttack;
	SERIALIZE GameObject mKillAttack;
	SERIALIZE GameObject mLockOnChange;
	SERIALIZE GameObject mGuard;
	SERIALIZE GameObject mAvoidance;


	SERIALIZE GameObject mGetWeapon;
	SERIALIZE GameObject mLockOn;
	SERIALIZE GameObject mBreakWeapon;
	SERIALIZE GameObject mTaru;
	SERIALIZE GameObject mHeal;



};