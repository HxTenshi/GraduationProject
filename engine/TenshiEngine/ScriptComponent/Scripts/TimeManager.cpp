#include "TimeManager.h"
#include <h_standard.h>


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Start(){
	m_isSlow = false;
	m_scaleTime = 1.0f;
}

//���t���[���Ă΂�܂�
void TimeManager::Update(){
	
	if (Input::Down(KeyCode::Key_Z)) {
		m_isSlow = true;
	}
	if (Input::Down(KeyCode::Key_X)) {
		m_isSlow = false;
	}
	//�f���^�^�C�����擾
	float deltaTime = Hx::DeltaTime()->GetNoScaleDeltaTime();

	//���b�K�v�Ƃ��邩
	float temp = 1.0f / m_slowRate;

	//�X���[���̏���
	if (IsSlow()) 
		m_scaleTime -= temp * deltaTime;

	//�ʏ펞�̏���
	else if(!IsSlow())
		m_scaleTime += temp* deltaTime;

	//0.1�`1.0f�Ɏ��܂�悤�ɃN�����v
	m_scaleTime = min(max(0.1f,m_scaleTime), 1.0f);
	//�l������
	Hx::DeltaTime()->SetTimeScale(m_scaleTime);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void TimeManager::Finish(){

}

void TimeManager::OnSlow() {
	m_isSlow = true;
}

void TimeManager::OffSlow(){
	m_isSlow = false;
}

bool TimeManager::IsSlow() {
	return m_isSlow;
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