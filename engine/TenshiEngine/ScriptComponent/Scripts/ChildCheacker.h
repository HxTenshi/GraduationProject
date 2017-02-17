
#pragma once
#include "main.h"


class ChildCheacker :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;

	int IsChildCount();

private:
	SERIALIZE GameObject mTarget;
	SERIALIZE bool mIsCheack;
	
	SERIALIZE bool mIsDead;
	SERIALIZE GameObject mDead;

};