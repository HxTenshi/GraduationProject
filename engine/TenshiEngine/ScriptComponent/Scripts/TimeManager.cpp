#include "TimeManager.h"
#include <h_standard.h>


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Start(){
	isSlow_ = false;
	scaleTime_ = 1.0f;
}

//���t���[���Ă΂�܂�
void TimeManager::Update(){
	
	if (Input::Down(KeyCode::Key_Z)) {
		isSlow_ = true;
	}
	if (Input::Down(KeyCode::Key_X)) {
		isSlow_ = false;
	}
	//�f���^�^�C�����擾
	float deltaTime = Hx::DeltaTime()->GetNoScaleDeltaTime();

	//���b�K�v�Ƃ��邩
	float temp = 1.0f / slowRate_;

	//�X���[���̏���
	if (IsSlow()) 
		scaleTime_ -= temp * deltaTime;

	//�ʏ펞�̏���
	else if(!IsSlow())
		scaleTime_ += temp* deltaTime;

	//0.1�`1.0f�Ɏ��܂�悤�ɃN�����v
	scaleTime_ = min(max(0.1f,scaleTime_), 1.0f);
	//�l������
	Hx::DeltaTime()->SetTimeScale(scaleTime_);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Finish(){

}

void TimeManager::OnSlow() {
	isSlow_ = true;
}

bool TimeManager::IsSlow() {
	return isSlow_;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TimeManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void TimeManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void TimeManager::OnCollideExit(GameObject target){
	(void)target;
}