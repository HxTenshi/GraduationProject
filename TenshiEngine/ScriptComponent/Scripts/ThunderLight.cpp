#include "ThunderLight.h"
#include "Game/Actor.h"
#include "Game/Component/DirectionalLightComponent.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ThunderLight::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ThunderLight::Start(){
	auto dir = gameObject->GetComponent<DirectionalLightComponent>();
	if (!dir)return;

	mColor = dir->GetColor();
}

//���t���[���Ă΂�܂�
void ThunderLight::Update(){
	auto dir = gameObject->GetComponent<DirectionalLightComponent>();
	if (!dir)return;

	float noize = (rand() % 100)/2000.0f;
	XMFLOAT3 noize3(noize, noize, noize);
	if (mThounderPow < 0.2){
		if (rand() % 2000 <= 10){
			mThounderPow = (rand() % 100) / 100.0f*0.5 + 0.5;
		}
	}
	mThounderPow -= 0.1;
	mThounderPow = max(mThounderPow, 0);
	XMFLOAT3 col = mColor;
	col.x += mThounderPow + noize;
	col.y += mThounderPow + noize;
	col.z += mThounderPow + noize;
	dir->SetColor(col);

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ThunderLight::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ThunderLight::OnCollideBegin(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ThunderLight::OnCollideEnter(Actor* target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ThunderLight::OnCollideExit(Actor* target){
	(void)target;
}