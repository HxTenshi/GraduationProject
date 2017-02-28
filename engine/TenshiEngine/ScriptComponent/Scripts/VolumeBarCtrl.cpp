#include "VolumeBarCtrl.h"
#include "h_standard.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void VolumeBarCtrl::Initialize(){
	m_widthScale = m_memoriBar->mTransform->Scale().x / VOLUME_RATE;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void VolumeBarCtrl::Start(){

}

//���t���[���Ă΂�܂�
void VolumeBarCtrl::Update(){
	//�N�����v����
	int minNum = 0;
	int maxNum = 100;
	m_volume = (int)min(max(minNum, m_volume), maxNum);

	XMVECTOR scale;
	scale.x = m_volume * m_widthScale;
	scale.y = m_memoriBar->mTransform->Scale().y;
	scale.z = 1.0f;

	m_memoriBar->mTransform->Scale(scale);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void VolumeBarCtrl::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void VolumeBarCtrl::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void VolumeBarCtrl::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void VolumeBarCtrl::OnCollideExit(GameObject target){
	(void)target;
}

void VolumeBarCtrl::VolumeUp(){
	m_volume++;
	m_volume = min(100.0f, m_volume);
}

void VolumeBarCtrl::VolumeDown(){
	m_volume--;
	m_volume = max(0.0f, m_volume);
}

//���ʂ��Z�b�g
void VolumeBarCtrl::SetVolume(int volume){
	m_volume = volume;
}

//���ʂ��擾
int VolumeBarCtrl::GetVolume(){
	return m_volume;
}

bool VolumeBarCtrl::IsVolumeMax()
{
	return m_volume >= VOLUME_RATE;
}

bool VolumeBarCtrl::IsVolumeMin()
{
	return m_volume <= 0.0f;
}
