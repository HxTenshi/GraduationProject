#include "AttackFilter.h"
#include"h_standard.h"
#include "Weapon.h"
#include "BombDamageEffect.h"

bool AttackFilter::IsAttack(GameObject attack, Filter filter)
{
	if (!attack)return false;
	bool hit = false;
	if (filter & Filter::eWeapon) {
		if (auto w = attack->GetScript<Weapon>()) {
			if (w->isAttack()) {
				hit = true;
			}
		}
	}
	if (filter & Filter::eBomb) {
		if (auto b = attack->GetScript<BombDamageEffect>()) {
			hit = true;
		}
	}
	return hit;
}
