
#pragma once
#include "main.h"


class AttackFilter :public IDllScriptComponent{
public:

	enum Filter {
		eWeapon = 0x0001,
		eBomb = 0x0002,
		eAll = 0xffff,
	};
	static bool IsAttack(GameObject attack, Filter filter = Filter::eAll);
private:
	//ƒƒ“ƒo•Ï”
};