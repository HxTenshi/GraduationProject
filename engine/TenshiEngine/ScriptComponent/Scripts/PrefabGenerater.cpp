#include "PrefabGenerater.h"

#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PrefabGenerater::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PrefabGenerater::Start(){

}

//毎フレーム呼ばれます
void PrefabGenerater::Update(){
	auto o = Hx::Instance(m_Object);
	if (o) {
		o->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
	}
	gameObject->RemoveComponent<ScriptComponent>();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PrefabGenerater::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PrefabGenerater::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PrefabGenerater::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PrefabGenerater::OnCollideExit(GameObject target){
	(void)target;
}