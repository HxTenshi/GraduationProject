#include "FireEffect.h"
#include "h_component.h"
#include "h_standard.h"
#include "Library/easing.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void FireEffect::Initialize(){
	m_Timer = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void FireEffect::Start(){

}

//毎フレーム呼ばれます
void FireEffect::Update(){

	auto com = gameObject->GetComponent<PointLightComponent>();
	if (com) {
		float t = 0;
		m_Timer += rand()/(float)RAND_MAX * m_RandomPower * Hx::DeltaTime()->GetDeltaTime();
		t = std::sin(m_Timer)/2.0f+0.5f;
		com->m_HDR = (float)Easing::Linear((double)t,1.0, (double)m_MaxHDR, (double)m_MinHDR);
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void FireEffect::Finish(){

}

//コライダーとのヒット時に呼ばれます
void FireEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void FireEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void FireEffect::OnCollideExit(GameObject target){
	(void)target;
}