#include "SandbagNose.h"
#include "Sandbag.h"
#include "../h_standard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SandbagNose::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void SandbagNose::Start(){

}

//���t���[���Ă΂�܂�
void SandbagNose::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void SandbagNose::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SandbagNose::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void SandbagNose::OnCollideEnter(GameObject target){
	(void)target;
	if (target->GetLayer() == 4) {
		gameObject->mTransform->GetParent()->mTransform->GetParent()->GetScript<Sandbag>()->SetHitWall(true);
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void SandbagNose::OnCollideExit(GameObject target){
	(void)target;
}