#include "testhit.h"
#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Graphic/Material/Material.h"
#include "Input/Input.h"
#include "Game/Script/IGame.h"

#include "PhysX/IPhysXEngine.h"
#include "engine/DebugEngine.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void testhit::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void testhit::Start(){

}

//���t���[���Ă΂�܂�
void testhit::Update() {

	gameObject->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(0,1,0,1));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void testhit::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void testhit::OnCollideBegin(GameObject target){

	auto mate = target->GetComponent<MaterialComponent>();
	if (!mate)return;
	auto m = mate->mAlbedo;
	m.z=0;
	m.x=0;
	mate->SetAlbedoColor(m);

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void testhit::OnCollideEnter(GameObject target){
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void testhit::OnCollideExit(GameObject target){
	auto mate = target->GetComponent<MaterialComponent>();
	if (!mate)return;
	auto m = mate->mAlbedo;
	m.z=1;
	m.x=1;
	mate->SetAlbedoColor(m);


}