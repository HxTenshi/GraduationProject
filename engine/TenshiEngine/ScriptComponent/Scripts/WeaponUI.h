
#pragma once
#include "main.h"
#include "Library\easing.h"
#include "WeaponCommon.h"
enum WeaponDamageType {
	Normal,
	Damage,
	Danger,
	Break
};
class WeaponUI :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void DurableGage(GameObject weapon);
	void SetWeaponIcon(WeaponType wtype,WeaponDamageType wdtype);
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	GameObject UI;
	SERIALIZE
	GameObject UIMix;

private:
	SERIALIZE
	GameObject whand;
	SERIALIZE
		GameObject m_weapon_ui_mem;
	//funifuni::Tween tween;
private:
	float nowhp;
	float maxhp;
private:
	//sword
	SERIALIZE
	TextureAsset m_sword_ui;
	SERIALIZE
	TextureAsset m_sword_damage_ui;
	SERIALIZE
	TextureAsset m_sword_danger_ui;
	SERIALIZE
	TextureAsset m_sword_break_ui;
	//axe
	SERIALIZE
	TextureAsset m_axe_ui;
	SERIALIZE
	TextureAsset m_axe_damage_ui;
	SERIALIZE
	TextureAsset m_axe_danger_ui;
	SERIALIZE
	TextureAsset m_axe_break_ui;
	//rance
	SERIALIZE
		TextureAsset m_rance_ui;
	SERIALIZE
		TextureAsset m_rance_damage_ui;
	SERIALIZE
		TextureAsset m_rance_danger_ui;
	SERIALIZE
		TextureAsset m_rance_break_ui;

	//rance
	SERIALIZE
		TextureAsset m_sworddh_ui;
	SERIALIZE
		TextureAsset m_sworddh_damage_ui;
	SERIALIZE
		TextureAsset m_sworddh_danger_ui;
	SERIALIZE
		TextureAsset m_sworddh_break_ui;
	//other
	SERIALIZE
		TextureAsset m_other_ui;



	SERIALIZE GameObject m_NameTextObject;

	
};
