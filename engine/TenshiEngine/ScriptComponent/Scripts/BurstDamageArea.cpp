#include "BurstDamageArea.h"

#include "PlayerController.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BurstDamageArea::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BurstDamageArea::Start(){

}

//���t���[���Ă΂�܂�
void BurstDamageArea::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void BurstDamageArea::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BurstDamageArea::OnCollideBegin(GameObject target){
	if (target) {
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = target->GetScript<PlayerController>();
		if (player) {
			auto v = target->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			player->Damage(m_Damege, XMVector3Normalize(v), m_KnockBackDonw ? PlayerController::KnockBack::Down : PlayerController::KnockBack::Low, m_DodgeInevitable, m_GuardInevitable);
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BurstDamageArea::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void BurstDamageArea::OnCollideExit(GameObject target){
	(void)target;
}