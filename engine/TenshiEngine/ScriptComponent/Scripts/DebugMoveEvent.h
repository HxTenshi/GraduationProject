
#pragma once
#include "main.h"
# include "h_standard.h"
#include "../h_component.h"
class DebugMoveEvent :public IDllScriptComponent {
public:
	void Update();
	void MoveToPont(GameObject target, KeyCode key);

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject mPoint_0;
	SERIALIZE GameObject mPoint_1;
	SERIALIZE GameObject mPoint_2;
	SERIALIZE GameObject mPoint_3;
	SERIALIZE GameObject mPoint_4;
};