#include "PlayerUI.h"
#include "PlayerController.h"
#include "h_component.h"
#include <sstream>

#include "h_standard.h"
#include "UniqueObject.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerUI::Initialize(){
	m_SP_Timer = 0.0f;
	m_HP_Timer = 0.0f;

	m_HP_Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_HP_Color_Red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_SP_Color = XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f);
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
	if(m_HP_Bar){
		float hp = scr->GetHP();
		float maxhp = scr->GetMaxHP();
		float hppar = hp / maxhp;
		float x = m_HP_BarSize * hppar;

		auto s = m_HP_Bar->mTransform->Scale();
		auto p = m_HP_Bar->mTransform->Position();
		p.x = -s.x * (1.0f - hppar);
		m_HP_Bar->mTransform->Position(p);

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
		p.x = -s.x * (1.0f - sppar);
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
	}

	auto hit = scr->GetHitComboCount();
	if (m_HIT_Text) {
		auto text = m_HIT_Text->GetComponent<TextComponent>();
		if (text) {
			if (hit >= 3) {
				auto t = std::to_string(hit) + "Hit";
				text->ChangeText(t);
			}
			else {
				text->ChangeText("");
			}
		}
	}
	{
		int move = 0;
		int wre = 0;
		int mou = 0;
		if (hit >= 0) {
		}
		if (hit >= 4) {
			mou++;

		}
		if (hit >= 5) {
			move++;

		}
		if (hit >= 8) {
			mou++;

		}
		if (hit >= 10) {
			wre++;
			move++;
		}
		if (hit >= 12) {
			mou++;

		}
		if (hit >= 15) {
			move++;

		}
		if (hit >= 16) {
			mou++;

		}
		if (hit >= 20) {
			wre++;
			move++;
			mou++;
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

		if (m_Combo_MoveUI) {
			auto mate = m_Combo_MoveUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(movecol[move]);
			}
		}
		if (m_Combo_WeaponResistUI) {
			auto mate = m_Combo_WeaponResistUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(wrecol[wre]);
			}
		}
		if (m_Combo_MoutionUI) {
			auto mate = m_Combo_MoutionUI->GetComponent<MaterialComponent>();
			if (mate) {
				mate->SetAlbedoColor(moucol[mou]);
			}
		}
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