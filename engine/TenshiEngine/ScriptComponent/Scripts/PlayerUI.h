
#pragma once
#include "main.h"

class TextComponent;
class PlayerUI :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ÉÅÉìÉoïœêî
	
	SERIALIZE float m_HP_BarSize;
	SERIALIZE GameObject m_HP_Bar;
	SERIALIZE GameObject m_HP_Bar_Damage;
	SERIALIZE float m_HP_Redline;
	XMFLOAT4 m_HP_Color;
	XMFLOAT4 m_HP_Color_Red;
	SERIALIZE float m_HP_BloomSpeed;
	SERIALIZE float m_HP_DamageWaitTime;
	SERIALIZE float m_HP_DamageDownSpeed;
	float m_HP_Timer;
	float m_HP_XOffset;
	float m_Last_HP;
	float m_HP_DamageTimer;

	SERIALIZE float m_SP_BarSize;
	SERIALIZE GameObject m_SP_Bar;
	SERIALIZE GameObject m_SP_Bar_Bloom;
	SERIALIZE float m_SP_BloomSpeed;
	SERIALIZE GameObject m_SP_B;
	SERIALIZE GameObject m_SP_Ball;
	SERIALIZE GameObject m_SP_Ball_Bloom;
	XMFLOAT4 m_SP_Color;
	float m_SP_Timer;
	float m_SP_XOffset;

	SERIALIZE GameObject m_HIT_Text;

	SERIALIZE GameObject m_Combo_MoveUI;
	SERIALIZE GameObject m_Combo_WeaponResistUI;
	SERIALIZE GameObject m_Combo_ATKUI;

	int m_LastCombo;
	SERIALIZE PrefabAsset m_Combo_MoveUI_UP;
	SERIALIZE PrefabAsset m_Combo_WeaponResistUI_UP;
	SERIALIZE PrefabAsset m_Combo_ATKUI_UP;

	SERIALIZE GameObject m_WeaponBreakUI;


	SERIALIZE float m_LockOnScaleTime;
	SERIALIZE float m_LockOnScaleMin;
	SERIALIZE float m_LockOnScaleMax;
	SERIALIZE GameObject m_LockOnUI;
	float m_LockOnScaleTimer;
};