#include "ReisCitrusBullet.h"

#include "h_standard.h"
#include "SoundManager.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ReisCitrusBullet::Initialize(){

	m_Vector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	m_SE_Timer = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ReisCitrusBullet::Start(){

}

//毎フレーム呼ばれます
void ReisCitrusBullet::Update(){
	auto pos = gameObject->mTransform->WorldPosition();
	pos += m_Vector * Hx::DeltaTime()->GetDeltaTime() * m_Speed;
	gameObject->mTransform->WorldPosition(pos);

	auto rot1 = gameObject->mTransform->WorldQuaternion();
	auto rot2 = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), m_RotateSpeed*Hx::DeltaTime()->GetDeltaTime() * m_RotateSpeed);
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(rot1,rot2));


	m_SE_Timer += Hx::DeltaTime()->GetDeltaTime();
	if (m_SE_Timer > 0.5f) {
		m_SE_Timer = fmod(m_SE_Timer, 0.5f);
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Citrus_Throw, gameObject->mTransform->WorldPosition());
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ReisCitrusBullet::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ReisCitrusBullet::OnCollideBegin(GameObject target){
	if (!target)return;
	if (target->GetLayer() == 4) {
		m_Vector = XMVectorZero();
	}
}

//コライダーとのヒット中に呼ばれます
void ReisCitrusBullet::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ReisCitrusBullet::OnCollideExit(GameObject target){
	(void)target;
}

void ReisCitrusBullet::SetVector(const XMVECTOR & vect)
{
	m_Vector = vect;
}
