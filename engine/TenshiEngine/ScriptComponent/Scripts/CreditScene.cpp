#include "CreditScene.h"
#include <h_standard.h>
#include <h_component.h>

//生成時に呼ばれます（エディター中も呼ばれます）
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

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreditScene::Start(){

}

//毎フレーム呼ばれます
void CreditScene::Update(){

	if (Input::Trigger(KeyCode::Key_RIGHT)) m_cursorNum++;
	if (Input::Trigger(KeyCode::Key_LEFT)) m_cursorNum--;
	int maxNum = m_objList.size() - 1;
	m_cursorNum = min(max(0, m_cursorNum),maxNum);
	Hx::Debug()->Log(std::to_string(m_cursorNum));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreditScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void CreditScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CreditScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CreditScene::OnCollideExit(GameObject target){
	(void)target;
}