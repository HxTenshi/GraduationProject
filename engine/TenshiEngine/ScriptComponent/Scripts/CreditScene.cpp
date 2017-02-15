#include "CreditScene.h"
#include <h_standard.h>
#include <h_component.h>

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreditScene::Initialize(){
	m_objList.push_back(m_weaponObj1);
	m_objList.push_back(m_weaponObj2);
	m_objList.push_back(m_weaponObj3);
	m_objList.push_back(m_weaponObj4);
	m_objList.push_back(m_weaponObj5);
	m_objList.push_back(m_weaponObj6);
	m_objList.push_back(m_weaponObj7);
	m_objList.push_back(m_weaponObj8);
	m_objList.push_back(m_weaponObj9);
	m_objList.push_back(m_weaponObj10);
	m_cursorNum = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreditScene::Start(){

}

//���t���[���Ă΂�܂�
void CreditScene::Update(){

	if (Input::Trigger(KeyCode::Key_RIGHT)) m_cursorNum++;
	if (Input::Trigger(KeyCode::Key_LEFT)) m_cursorNum--;
	int maxNum = m_objList.size() - 1;
	m_cursorNum = min(max(0, m_cursorNum),maxNum);
	Hx::Debug()->Log(std::to_string(m_cursorNum));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CreditScene::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreditScene::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreditScene::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void CreditScene::OnCollideExit(GameObject target){
	(void)target;
}