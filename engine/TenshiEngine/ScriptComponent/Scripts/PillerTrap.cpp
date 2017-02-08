#include "PillerTrap.h"
#include "PlayerController.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PillerTrap::Initialize(){
	is_action = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void PillerTrap::Start(){

}

//���t���[���Ă΂�܂�
void PillerTrap::Update(){
	m_time += Hx::DeltaTime()->GetDeltaTime();
	if (m_time>m_action_time) {
		is_action = true;
		Action();
	}
	Retract();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void PillerTrap::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PillerTrap::OnCollideBegin(GameObject target){
	if (target) {
		auto player = target->GetScript<PlayerController>();
		if (player) {
			player->Damage(m_damage, XMVectorZero(), PlayerController::KnockBack::None, true, true);
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void PillerTrap::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
