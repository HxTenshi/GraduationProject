//Weapon‚ª“G‚É“–‚½‚Á‚½‚Æ‚«‚Æ‚©‚ÉŒÄ‚ÔŠÖ”‚ğ’è‹`‚µ‚Ä‚¨‚­B

#pragma once
#include "main.h"


class WeaponControl :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	//Weapon‚ÌBigin‚ÅŒÄ‚ñ‚Å‚à‚ç‚¤
	void HitActor(GameObject target, GameObject weapon);
	void IsEnemy(GameObject target, GameObject weapon);

private:
	//ƒƒ“ƒo•Ï”
};