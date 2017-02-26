
#pragma once
#include "main.h"


class ChangeEventPlayerStatus :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

	void OnSetSP();

private:
	SERIALIZE float mSP;
};