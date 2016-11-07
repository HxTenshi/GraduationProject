#include "PlayerUI.h"
#include "PlayerController.h"
#include "h_component.h"
#include <sstream>

#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerUI::Initialize(){
	m_TextComponent = NULL;
	m_TextComponent = gameObject->GetComponent<TextComponent>();
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PlayerUI::Start(){

}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PlayerUI::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerUI::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PlayerUI::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void PlayerUI::OnCollideExit(GameObject target){
	(void)target;
}