#include "ParticleAutoDelete.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParticleAutoDelete::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ParticleAutoDelete::Start(){

}

//���t���[���Ă΂�܂�
void ParticleAutoDelete::Update(){
	count_auto_delete_time += Hx::DeltaTime()->GetDeltaTime();
	if (auto_delete_time <= count_auto_delete_time)Hx::DestroyObject(gameObject);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ParticleAutoDelete::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParticleAutoDelete::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ParticleAutoDelete::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ParticleAutoDelete::OnCollideExit(GameObject target){
	(void)target;
}