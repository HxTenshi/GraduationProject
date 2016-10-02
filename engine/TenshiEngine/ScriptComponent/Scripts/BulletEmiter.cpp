#include "BulletEmiter.h"

#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Input/Input.h"
#include "Game/Script/IGame.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BulletEmiter::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void BulletEmiter::Start(){

}

//���t���[���Ă΂�܂�
void BulletEmiter::Update(){
	if (Input::Trigger(KeyCoord::Key_Z)){
		auto actor = Hx::Instance(mEmitObject.c_str());
		if (!actor)return;
		actor->mTransform->Position(gameObject->mTransform->Position());
		actor->mTransform->Rotate(gameObject->mTransform->Rotate());
	}
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void BulletEmiter::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BulletEmiter::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void BulletEmiter::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void BulletEmiter::OnCollideExit(GameObject target){
	(void)target;
}