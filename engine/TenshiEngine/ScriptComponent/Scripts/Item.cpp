#include "Item.h"
#include "PlayerController.h"
#include "h_standard.h"
#include "SoundManager.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Item::Initialize(){
	m_Time = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Item::Start(){

	m_StartPos = gameObject->mTransform->WorldPosition();
}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Item::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Item::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Item::OnCollideExit(GameObject target){
	(void)target;
}