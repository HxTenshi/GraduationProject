#include "PlayerUI.h"
#include "PlayerController.h"
#include "h_component.h"
#include <sstream>

#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerUI::Initialize(){
	m_TextComponent = NULL;
	m_TextComponent = gameObject->GetComponent<TextComponent>();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerUI::Start(){

}

//毎フレーム呼ばれます
void PlayerUI::Update(){
	if (!m_Player)return;
	if (!m_TextComponent) {
		m_TextComponent = gameObject->GetComponent<TextComponent>();
		if (!m_TextComponent)return;
	}
	auto scr = m_Player->GetScript<PlayerController>();
	if (!scr)return;
	std::stringstream text;
	float hp = scr->GetHP();
	float maxhp = scr->GetMaxHP();
	float sp = scr->GetSpecial();
	text << "HP:" << hp << "/" << maxhp;
	text << ",SP:" << sp;
	m_TextComponent->ChangeText(text.str());
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