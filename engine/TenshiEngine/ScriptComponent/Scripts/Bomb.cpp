#include "Bomb.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Bomb::Initialize(){
	started_flag = false;
	is_start = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Bomb::Start(){

}

//���t���[���Ă΂�܂�
void Bomb::Update(){
	//��Ɏ����Ă����ԂŋN�����Ă��Ȃ�������
	if (m_weapon_object->GetScript<Weapon>()->GetIsHand() && !is_start) {
		is_start = true;
	}
	if (is_start) {
		CountDown();
		Explosion();
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Bomb::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Bomb::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Bomb::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Bomb::OnCollideExit(GameObject target){
	(void)target;
}

void Bomb::CountDown()
{
	if (m_explosion_time <= 0) {
		m_explosion_time = 0;
		return;
	}
	m_explosion_time -= Hx::DeltaTime()->GetDeltaTime();
}

void Bomb::Explosion()
{
	if (m_explosion_time<=0) {

		//��������
		Hx::DestroyObject(m_weapon_object);
	}
}
