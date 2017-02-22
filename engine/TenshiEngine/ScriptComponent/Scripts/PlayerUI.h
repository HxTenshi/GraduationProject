
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
	SERIALIZE float m_HP_Redline;
	XMFLOAT4 m_HP_Color;
	XMFLOAT4 m_HP_Color_Red;
	SERIALIZE float m_HP_BloomSpeed;
	float m_HP_Timer;

	SERIALIZE float m_SP_BarSize;
	SERIALIZE GameObject m_SP_Bar;
	SERIALIZE GameObject m_SP_Bar_Bloom;
	SERIALIZE float m_SP_BloomSpeed;
	XMFLOAT4 m_SP_Color;
	float m_SP_Timer;

	SERIALIZE GameObject m_HIT_Text;

	SERIALIZE GameObject m_Combo_MoveUI;
	SERIALIZE GameObject m_Combo_WeaponResistUI;
	SERIALIZE GameObject m_Combo_MoutionUI;
};