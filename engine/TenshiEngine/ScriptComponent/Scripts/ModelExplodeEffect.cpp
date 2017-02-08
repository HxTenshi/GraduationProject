#include "ModelExplodeEffect.h"
#include "h_standard.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ModelExplodeEffect::Initialize(){
	if (expModel1)objects.push_back(expModel1);
	if (expModel2)objects.push_back(expModel2);
	if (expModel3)objects.push_back(expModel3);
	if (expModel4)objects.push_back(expModel4);
	if (expModel5)objects.push_back(expModel5);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ModelExplodeEffect::Start(){
	time = 0.0f;
	count_auto_delete_time = 0.0f;
	explode_flag = false;
	for (auto i : objects) {
		i->Disable();
	}
}

//���t���[���Ă΂�܂�
void ModelExplodeEffect::Update(){
	Timer();
	ExplodeFunc();
	AutoDeleteTimer();
	AutoDelete();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ModelExplodeEffect::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ModelExplodeEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ModelExplodeEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ModelExplodeEffect::OnCollideExit(GameObject target){
	(void)target;
}

void ModelExplodeEffect::Timer()
{
	time += Hx::DeltaTime()->GetDeltaTime();
}

void ModelExplodeEffect::ExplodeFunc()
{
	if (explode_flag)return;
	if (time >= explode_time) {
		for (auto i : objects) {
			i->Enable();
		}
		explode_flag = true;
	}
}

void ModelExplodeEffect::AutoDeleteTimer()
{
	count_auto_delete_time += Hx::DeltaTime()->GetDeltaTime();
}

void ModelExplodeEffect::AutoDelete()
{
	if (count_auto_delete_time >= auto_delete_time) {
		Hx::DestroyObject(gameObject);
	}
}
