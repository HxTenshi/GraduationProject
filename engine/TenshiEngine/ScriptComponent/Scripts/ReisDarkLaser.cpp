#include "ReisDarkLaser.h"

#include "UniqueObject.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ReisDarkLaser::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ReisDarkLaser::Start(){

}

//毎フレーム呼ばれます
void ReisDarkLaser::Update(){
	auto player = UniqueObject::GetPlayer();
	if (!player)return;

	auto v = player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	v.y = 0.0f;
	if (XMVector3Length(v).x != 0.0f) {
		v = XMVector3Normalize(v);
	}
	v = v * m_Speed * Hx::DeltaTime()->GetDeltaTime();

	auto pos = gameObject->mTransform->WorldPosition();
	pos += v;
	gameObject->mTransform->WorldPosition(pos);

	if (m_BoundEffect) {
		RaycastHit hit;
		if (Hx::PhysX()->RaycastHit(pos, XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f), 40.0f, &hit, Layer::UserTag4)) {
			m_BoundEffect->mTransform->WorldPosition(hit.position);
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ReisDarkLaser::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ReisDarkLaser::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ReisDarkLaser::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ReisDarkLaser::OnCollideExit(GameObject target){
	(void)target;
}