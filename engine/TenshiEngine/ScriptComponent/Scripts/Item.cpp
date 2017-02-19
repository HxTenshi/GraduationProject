#include "Item.h"
#include "PlayerController.h"
#include "h_standard.h"
#include "SoundManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Item::Initialize(){
	m_Time = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Item::Start(){

	m_StartPos = gameObject->mTransform->WorldPosition();
}

//毎フレーム呼ばれます
void Item::Update(){

	auto t = Hx::DeltaTime()->GetDeltaTime();
	m_Time += t;

	auto rot = XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 1), m_RotY * t);
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(gameObject->mTransform->WorldQuaternion(), rot));

	if (m_MoveY_Time != 0.0f) {
		m_Time = fmod(m_Time, m_MoveY_Time);

		auto p = m_StartPos;
		p.y += std::sin(m_Time*XM_2PI) * m_MoveY;
		gameObject->mTransform->WorldPosition(p);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Item::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Item::OnCollideBegin(GameObject target){
	if (!target)return;


	if (auto player = target->GetScript<PlayerController>()) {
		player->AddHP(m_AddHP);
		player->AddMaxHP(m_AddMaxHP);
		player->AddSpecial(m_AddSP);
		if (m_GetEffect.IsLoad()) {
			if (auto obj = Hx::Instance(m_GetEffect)) {
				obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
			}
		}
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Recovery, gameObject->mTransform->WorldPosition());
		Hx::DestroyObject(gameObject);

	}
}

//コライダーとのヒット中に呼ばれます
void Item::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Item::OnCollideExit(GameObject target){
	(void)target;
}