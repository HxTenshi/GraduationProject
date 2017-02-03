#include "ReisDarkLaser.h"

#include "UniqueObject.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisDarkLaser::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisDarkLaser::Start(){

}

//���t���[���Ă΂�܂�
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ReisDarkLaser::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisDarkLaser::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisDarkLaser::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ReisDarkLaser::OnCollideExit(GameObject target){
	(void)target;
}