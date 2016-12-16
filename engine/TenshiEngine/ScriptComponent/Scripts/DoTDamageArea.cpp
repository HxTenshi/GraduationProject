#include "DoTDamageArea.h"

#include "h_standard.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void DoTDamageArea::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void DoTDamageArea::Start(){

}

//毎フレーム呼ばれます
void DoTDamageArea::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void DoTDamageArea::Finish(){

}

//コライダーとのヒット時に呼ばれます
void DoTDamageArea::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void DoTDamageArea::OnCollideEnter(GameObject target){
	if (target) {
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = target->GetScript<PlayerController>();
		if (player) {
			player->Damage(m_1secDamege * time, XMVectorZero());
		}
	}
}

//コライダーとのロスト時に呼ばれます
void DoTDamageArea::OnCollideExit(GameObject target){
	(void)target;
}