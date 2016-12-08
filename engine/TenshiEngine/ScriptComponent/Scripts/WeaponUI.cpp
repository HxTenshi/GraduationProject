#include "WeaponUI.h"

#include "WeaponHand.h"
#include "Library\easing.h"
#include "h_standard.h"
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponUI::Initialize(){
	UI->GetComponent<TextureModelComponent>()->SetTexture(m_sword_ui);

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponUI::Start(){
	if (whand->GetScript<WeaponHand>()->GetHandWeapon()) {
		UIMix->Enable();
	}
	else {
		UIMix->Disable();
	}
}

//毎フレーム呼ばれます
void WeaponUI::Update(){
	GameObject weapon;
	//time += Hx::DeltaTime()->GetDeltaTime();
	if (weapon=whand->GetScript<WeaponHand>()->GetHandWeapon()) {
		UIMix->Enable();
		if (Input::Trigger(KeyCode::Key_7)) {
			weapon->GetScript<Weapon>()->Damage(DamageType::LowDamage, 1);
		}
			DurableGage(weapon);
	}
	else {
		UIMix->Disable();
	}

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponUI::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponUI::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponUI::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponUI::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponUI::DurableGage(GameObject weapon)
{
	if (weapon) {
		float maxdurable = weapon->GetScript<Weapon>()->GetMaxDurable();
		float nowdurable = weapon->GetScript<Weapon>()->GetDurable();
		WeaponType type = weapon->GetScript<Weapon>()->GetWeaponType();
		float damage = maxdurable - (maxdurable - nowdurable);
		float max = 1;
		float mul = nowdurable / maxdurable;
		float half = maxdurable / 2;
		float danger = maxdurable*(2.0f / 10.0f);
		SetWeaponIcon(type, WeaponDamageType::Normal);
		if (half >= nowdurable) {
			//m_DurableGage->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f));
			SetWeaponIcon(type, WeaponDamageType::Damage);
		}
		if (danger >= nowdurable) {
			//m_DurableGage->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(0.698f, 0.133f, 0.133f, 1.0f));
			SetWeaponIcon(type, WeaponDamageType::Danger);
		}
		if (0 >= nowdurable) {
			//m_DurableGage->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
			//nowdurable = 1.0f;
			//mul = nowdurable / maxdurable;
			SetWeaponIcon(type, WeaponDamageType::Break);
		}
		//float gage = 100 * mul*6.0f;
		//tween.SetTween(gage,maxdurable,0.1f);
		//XMVECTOR  scale = m_DurableGage->mTransform->Scale();
		//m_DurableGage->mTransform->Scale(XMVectorSet(gage, scale.y, scale.z, scale.w));
	}
}

void WeaponUI::SetWeaponIcon(WeaponType wtype,WeaponDamageType wdtype)
{
	TextureAsset result=m_sword_ui;
	if (wtype == WeaponType::Sword) {
		if (wdtype == WeaponDamageType::Normal)result = m_sword_ui;
		if (wdtype == WeaponDamageType::Damage)result = m_sword_damage_ui;
		if (wdtype == WeaponDamageType::Danger)result = m_sword_danger_ui;
		if (wdtype == WeaponDamageType::Break)result = m_sword_break_ui;
	}
	if (wtype == WeaponType::Rance) {
		if (wdtype == WeaponDamageType::Normal)result = m_rance_ui;
		if (wdtype == WeaponDamageType::Damage)result = m_rance_damage_ui;
		if (wdtype == WeaponDamageType::Danger)result = m_rance_danger_ui;
		if (wdtype == WeaponDamageType::Break)result = m_rance_break_ui;
	}
	if (wtype == WeaponType::Axe) {
		if (wdtype == WeaponDamageType::Normal)result = m_axe_ui;
		if (wdtype == WeaponDamageType::Damage)result = m_axe_damage_ui;
		if (wdtype == WeaponDamageType::Danger)result = m_axe_danger_ui;
		if (wdtype == WeaponDamageType::Break)result = m_axe_break_ui;
	}
	UI->GetComponent<TextureModelComponent>()->SetTexture(result);
}
