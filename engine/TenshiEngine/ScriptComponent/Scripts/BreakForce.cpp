#include "BreakForce.h"

#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void BreakForce::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void BreakForce::Start(){

	GameObject obj;

	obj = m_Object1;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force,ForceMode::eIMPULSE);
		}
	}
	obj = m_Object2;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object3;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object4;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object5;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
	obj = m_Object6;
	if (obj) {
		if (auto com = obj->GetComponent<PhysXComponent>()) {
			auto v = obj->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			com->AddForce(XMVector3Normalize(v)*m_Force, ForceMode::eIMPULSE);
		}
	}
}

//毎フレーム呼ばれます
void BreakForce::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void BreakForce::Finish(){

}

//コライダーとのヒット時に呼ばれます
void BreakForce::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void BreakForce::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void BreakForce::OnCollideExit(GameObject target){
	(void)target;
}