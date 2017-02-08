#include "PillerTrap.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void PillerTrap::Initialize(){
	is_action = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PillerTrap::Start(){

}

//毎フレーム呼ばれます
void PillerTrap::Update(){
	m_time += Hx::DeltaTime()->GetDeltaTime();
	if (m_time>m_action_time) {
		is_action = true;
		Action();
	}
	Retract();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PillerTrap::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PillerTrap::OnCollideBegin(GameObject target){
	if (target) {
		auto player = target->GetScript<PlayerController>();
		if (player) {
			player->Damage(m_damage, XMVectorZero(), PlayerController::KnockBack::None, true, true);
		}
	}
}

//コライダーとのヒット中に呼ばれます
void PillerTrap::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PillerTrap::OnCollideExit(GameObject target){
	(void)target;
}

void PillerTrap::Check()
{
	if (m_time > m_action_time) {
		is_action = true;
	}
}

void PillerTrap::Action()
{
	if (!is_action)return;
	auto pos = gameObject->mTransform->Position();
	if (pos.y > up) {
		is_action = true;
		m_time=0.0f;
		return;
	}
	pos.y += Hx::DeltaTime()->GetDeltaTime()*30.0f;
	gameObject->mTransform->Position(pos);

}

void PillerTrap::Retract()
{
	if (is_action&m_time<=0.5f)return;
	auto pos = gameObject->mTransform->Position();
	if (pos.y < 0.0f) {
		is_action = false;
		m_time = 0.0f;
		return;
	}
	pos.y -= Hx::DeltaTime()->GetDeltaTime()*4.0f;
	gameObject->mTransform->Position(pos);
}
