#include "CreditDraw.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreditDraw::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void CreditDraw::Start(){

}

//���t���[���Ă΂�܂�
void CreditDraw::Update(){
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void CreditDraw::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreditDraw::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void CreditDraw::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void CreditDraw::OnCollideExit(GameObject target){
	(void)target;
}

TextureAsset CreditDraw::GetTexture()
{
	return m_texture;
}