#include "ModelEffectController.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ModelEffectController::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ModelEffectController::Start(){
	time = 0;
	delete_time = 0;
}

//���t���[���Ă΂�܂�
void ModelEffectController::Update(){
	UpperFunc();
	AutoDelete();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ModelEffectController::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ModelEffectController::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ModelEffectController::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ModelEffectController::OnCollideExit(GameObject target){
	(void)target;
}

void ModelEffectController::UpperFunc()
{
	if (!isUpper)return;
	time += Hx::DeltaTime()->GetDeltaTime();
	if (OverCheck())return;
	if (start_time <= time) {
		auto pos = ModelEff->mTransform->Position();
		pos.y += Hx::DeltaTime()->GetDeltaTime()*speed;
		ModelEff->mTransform->Position(pos);
	}
}

bool ModelEffectController::OverCheck()
{
	if (ModelEff->mTransform->Position().y > up-0.1f) {
		auto pos = ModelEff->mTransform->Position();
		pos.y = up;
		ModelEff->mTransform->Position(pos);
		return true;
	}
	return false;
}

void ModelEffectController::AutoDelete()
{
	delete_time += Hx::DeltaTime()->GetDeltaTime();
	if (delete_time >= auto_delete_time) {
		ModelEff->mTransform->Disable();
	}
	if (delete_time+3 >= auto_delete_time) {
		Hx::DestroyObject(gameObject);
	}
}
