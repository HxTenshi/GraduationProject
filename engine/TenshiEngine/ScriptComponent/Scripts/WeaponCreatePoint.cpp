#include "WeaponCreatePoint.h"
#include <random>

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponCreatePoint::Initialize(){
	
		auto g=Hx::Instance(weapon);
		g->mTransform->SetParent(gameObject);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponCreatePoint::Start(){

}

//���t���[���Ă΂�܂�
void WeaponCreatePoint::Update(){

}z	

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponCreatePoint::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponCreatePoint::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponCreatePoint::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponCreatePoint::OnCollideExit(GameObject target){
	(void)target;
}