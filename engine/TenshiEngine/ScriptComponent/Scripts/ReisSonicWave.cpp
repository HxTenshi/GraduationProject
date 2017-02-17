#include "ReisSonicWave.h"

#include "UniqueObject.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisSonicWave::Initialize(){
	m_Timer = 0.0f;
	m_HitOnFrame = 0;
	m_GenEnd = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisSonicWave::Start(){

}

//���t���[���Ă΂�܂�
void ReisSonicWave::Update(){
	m_Timer += Hx::DeltaTime()->GetDeltaTime();

	if (m_WaitTime <= m_Timer) {
		if (m_HitBox && !m_GenEnd) {
			m_GenEnd = true;
			m_HitBox->Enable();

			if (auto obj = Hx::Instance(m_WaveEffect)) {
				auto pos = gameObject->mTransform->WorldPosition();
				pos += gameObject->mTransform->Forward() * 50.0f;
				obj->mTransform->WorldPosition(pos);
				obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());
			}
		}
		m_HitOnFrame++;
	}
	if (m_WaitTime != 0.0f && m_HintBox) {
		auto s = 1.0f - m_Timer / m_WaitTime;
		s = max(s, 0.0f);
		s *= 0.5f;
		m_HintBox->mTransform->Scale(XMVectorSet(s, s, 100.0f, 1.0f));
	}
	if (m_HitOnFrame >= 3) {
		Hx::DestroyObject(gameObject);
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ReisSonicWave::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisSonicWave::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisSonicWave::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ReisSonicWave::OnCollideExit(GameObject target){
	(void)target;
}

void ReisSonicWave::PlayerLockOn()
{
	if (m_GenEnd) {
		return;
	}
	if (auto player = UniqueObject::GetPlayer()) {
		auto p = player->mTransform->WorldPosition();
		auto v = p - gameObject->mTransform->WorldPosition();
		auto vect = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
		v.y = 0.0f;
		if (XMVector3Length(v).x != 0.0f) {
			vect = XMVector3Normalize(v);
		}

		auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 1), vect, XMVectorSet(0, 1, 0, 1)));
		auto q = XMQuaternionRotationMatrix(rotate);

		gameObject->mTransform->WorldQuaternion(q);
	}
}
