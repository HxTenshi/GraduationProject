#include "PlayerUI.h"
#include "PlayerController.h"
#include "h_component.h"
#include <sstream>

#include "h_standard.h"
#include "UniqueObject.h"
#include "Weapon.h"
#include "Library\easing.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerUI::Initialize(){
	m_SP_Timer = 0.0f;
	m_HP_Timer = 0.0f;

	m_HP_Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_HP_Color_Red = XMFLOAT4(1.0f, 4.0f, 2.0f, 1.0f);
	m_SP_Color = XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f);
	m_HP_XOffset = 0.0f;
	m_SP_XOffset = 0.0f;

	m_Last_HP = 0.0f;
	m_HP_DamageTimer = 0.0f;

	m_LastCombo = 0;

	m_LockOnScaleTimer = 0.0f;

	if (m_HP_Bar) {
		m_HP_XOffset = m_HP_Bar->mTransform->Position().x;
	}
	if (m_SP_Bar) {
		m_SP_XOffset = m_SP_Bar->mTransform->Position().x;
	}
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerUI::Start(){

}

//毎フレーム呼ばれます
void PlayerUI::Update(){
	auto m_Player = UniqueObject::GetPlayer();
	if (!m_Player)return;
	auto scr = m_Player->GetScript<PlayerController>();
	if (!scr)return;
	if (m_HP_Bar) {
		float hp = scr->GetHP();
		float maxhp = scr->GetMaxHP();
		float hppar = hp / maxhp;
		float x = m_HP_BarSize * hppar;

		auto s = m_HP_Bar->mTransform->Scale();
		auto p = m_HP_Bar->mTransform->Position();
		p.x = -s.x * (1.0f - hppar) + m_HP_XOffset;
		m_HP_Bar->mTransform->Position(p);

		if (m_Last_HP > hp) {
			m_HP_DamageTimer = m_HP_DamageWaitTime;
		}
		m_Last_HP = hp;

		if (m_HP_DamageTimer > 0.0f) {
			m_HP_DamageTimer -= Hx::DeltaTime()->GetDeltaTime();
			m_HP_DamageTimer = max(m_HP_DamageTimer, 0.0f);
		}
		else {
			auto dp = m_HP_Bar_Damage->mTransform->Position();
			dp.x -= m_HP_DamageDownSpeed * Hx::DeltaTime()->GetDeltaTime();
			p.x = max(dp.x, p.x-1.0f);
			p.z = dp.z;
			m_HP_Bar_Damage->mTransform->Position(p);
		}


		if (m_HP_BloomSpeed == 0.0f)return;
		m_HP_Timer += Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_HP_Timer = fmod(m_HP_Timer, m_HP_BloomSpeed);
		auto t = m_HP_Timer / m_HP_BloomSpeed;
		t = (std::sin(t*XM_2PI) + 1.0f) / 2.0f;

		auto mate = m_HP_Bar->GetComponent<MaterialComponent>();
		if (mate) {
			if (hppar <= m_HP_Redline) {
				m_HP_Color_Red.w = t;
				mate->SetAlbedoColor(m_HP_Color_Red);
			}else{
				mate->SetAlbedoColor(m_HP_Color);
				m_HP_Timer = 0.0f;
			}
		}
	}
	if(m_SP_Bar){

		float sp = scr->GetSpecial();
		float sppar = sp / 100.0f;
		float x = m_SP_BarSize * sppar;

		auto s = m_SP_Bar->mTransform->Scale();
		auto p = m_SP_Bar->mTransform->Position();
		p.x = -s.x * (1.0f - sppar) + m_SP_XOffset;
		m_SP_Bar->mTransform->Position(p);

		if (m_SP_BloomSpeed == 0.0f)return;
		m_SP_Timer += Hx::DeltaTime()->GetNoScaleDeltaTime();
		m_SP_Timer = fmod(m_SP_Timer, m_SP_BloomSpeed);
		auto t = m_SP_Timer / m_SP_BloomSpeed;
		t = (std::sin(t*XM_2PI)+1.0f) / 2.0f;
		if (m_SP_Bar_Bloom) {
			auto mate = m_SP_Bar_Bloom->GetComponent<MaterialComponent>();
			if (mate) {
				if (sppar >= 1.0f) {
					m_SP_Color.w = t;
					mate->SetAlbedoColor(m_SP_Color);
				}
				else {
					m_SP_Color.w = 0.0f;
					mate->SetAlbedoColor(m_SP_Color);
					m_SP_Timer = 0.0f;
				}
			}
		}
		if (m_SP_Ball_Bloom && m_SP_Ball && m_SP_B) {
			auto mate = m_SP_Ball_Bloom->GetComponent<MaterialComponent>();
			if (mate) {
				if (sppar >= 1.0f) {
					m_SP_B->Enable();
					m_SP_Ball->Enable();
					m_SP_Color.w = t;
					mate->SetAlbedoColor(m_SP_Color);
				}
				else {
					m_SP_B->Disable();
					m_SP_Ball->Disable();
					m_SP_Color.w = 0.0f;
					mate->SetAlbedoColor(m_SP_Color);
					m_SP_Timer = 0.0f;
				}
			}
		}
	}

	auto combo = scr->GetHitComboCount();
	if (m_HIT_Text) {
		auto text = m_HIT_Text->GetComponent<TextComponent>();
		if (text) {
			if (combo >= 3) {
				m_HIT_Text->Enable();
				combo = min(combo,999);
				auto t = std::to_string(combo);
				for (int i = 0; i < 3 - t.size(); i++) {
					t = "0" + t;
				}
				text->ChangeText(t);
			}
			else {
				m_HIT_Text->Disable();
			}
		}
	}
	{
		int move[2] = { 0,0 };
		int wre[2] = { 0,0 };
		int atk[2] = { 0,0 };
		for (int i = 0; i < 2; i++) {
			int hit = 0;
			if (i == 0) {
				hit = combo;
			}
			else {
				hit = m_LastCombo;
			}
			if (hit >= 0) {
			}
			if (hit >= 4) {
				move[i]++;

			}
			if (hit >= 5) {
				atk[i]++;

			}
			if (hit >= 8) {
				move[i]++;

			}
			if (hit >= 10) {
				wre[i]++;
				atk[i]++;
			}
			if (hit >= 12) {
				move[i]++;

			}
			if (hit >= 15) {
				atk[i]++;

			}
			if (hit >= 16) {
				move[i]++;

			}
			if (hit >= 20) {
				wre[i]++;
				move[i]++;
				atk[i]++;
			}
		}
		const XMFLOAT4 movecol[5] = {
			XMFLOAT4(1.0f,1.0f,1.0f,0.5f),
			XMFLOAT4(0.0f,0.0f,1.0f,1.0f),
			XMFLOAT4(1.0f,1.0f,0.0f,1.0f),
			XMFLOAT4(0.6f,0.4f,0.0f,1.0f),
			XMFLOAT4(1.0f,0.0f,0.0f,1.0f),
		};
		const XMFLOAT4 moucol[6] = {
			XMFLOAT4(1.0f,1.0f,1.0f,0.5f),
			XMFLOAT4(0.0f,0.0f,1.0f,1.0f),
			XMFLOAT4(0.0f,1.0f,0.0f,1.0f),
			XMFLOAT4(1.0f,1.0f,0.0f,1.0f),
			XMFLOAT4(0.6f,0.4f,0.0f,1.0f),
			XMFLOAT4(1.0f,0.0f,0.0f,1.0f),
		};
		const XMFLOAT4 wrecol[3] = {
			XMFLOAT4(1.0f,1.0f,1.0f,0.5f),
			XMFLOAT4(0.0f,0.0f,1.0f,1.0f),
			XMFLOAT4(1.0f,0.0f,0.0f,1.0f),
		};
		if (move[0] > move[1]) {
			if (m_Combo_MoveUI_UP.IsLoad()) {
				Hx::Instance(m_Combo_MoveUI_UP);
			}
		}
		if (wre[0] > wre[1]) {
			if (m_Combo_WeaponResistUI_UP.IsLoad()) {
				Hx::Instance(m_Combo_WeaponResistUI_UP);
			}
		}
		if (atk[0] > atk[1]) {
			if (m_Combo_ATKUI_UP.IsLoad()) {
				Hx::Instance(m_Combo_ATKUI_UP);
			}
		}

		if (m_Combo_MoveUI) {
			auto mate = m_Combo_MoveUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(movecol[move[0]]);
			}
		}
		if (m_Combo_WeaponResistUI) {
			auto mate = m_Combo_WeaponResistUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(wrecol[wre[0]]);
			}
		}
		if (m_Combo_ATKUI) {
			auto mate = m_Combo_ATKUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(moucol[atk[0]]);
			}
		}

		m_LastCombo = combo;
	}


	if (auto w = scr->GetWeapon()) {
		if (m_WeaponBreakUI) {
			if (w->isBreak()) {
				m_WeaponBreakUI->Enable();
			}
			else {
				m_WeaponBreakUI->Disable();
			}
		}
	}

	auto target = scr->GetLockonTarget();
	if (target && m_LockOnScaleTime !=0.0f && Enemy::GetEnemy(target) && m_LockOnUI){
		m_LockOnUI->Enable();
		m_LockOnScaleTimer += Hx::DeltaTime()->GetDeltaTime();
		m_LockOnScaleTimer = fmodf(m_LockOnScaleTimer, m_LockOnScaleTime);

		float t = m_LockOnScaleTimer / m_LockOnScaleTime;
		t *= 2;
		float s = 1.0f;
		if (t < 1) {
			s = Easing::InExp(t, 1.0f, m_LockOnScaleMax, m_LockOnScaleMin);
		}
		else {
			t -= 1;
			s = Easing::OutExp(t, 1.0f, m_LockOnScaleMin, m_LockOnScaleMax);
		}

		m_LockOnUI->mTransform->Scale(XMVectorSet(s,s,1.0f, 1.0f));
	}
	else if(m_LockOnUI){
		m_LockOnUI->Disable();
	}
	
	
	//std::stringstream text;
	//text << "HP:" << hp << "/" << maxhp;
	//text << ",SP:" << sp;
	//m_TextComponent->ChangeText(text.str());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerUI::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PlayerUI::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PlayerUI::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PlayerUI::OnCollideExit(GameObject target){
	(void)target;
}