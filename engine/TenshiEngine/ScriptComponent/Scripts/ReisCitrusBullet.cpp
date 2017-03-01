#include "ReisCitrusBullet.h"

#include "h_standard.h"
#include "SoundManager.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisCitrusBullet::Initialize(){

	m_Vector = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	m_SE_Timer = 0.0f;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisCitrusBullet::Start(){

}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ReisCitrusBullet::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisCitrusBullet::OnCollideBegin(GameObject target){
	if (!target)return;
	if (target->GetLayer() == 4) {
		m_Vector = XMVectorZero();
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisCitrusBullet::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ReisCitrusBullet::OnCollideExit(GameObject target){
	(void)target;
}

void ReisCitrusBullet::SetVector(const XMVECTOR & vect)
{
	m_Vector = vect;
}
