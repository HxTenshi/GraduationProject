#include "MapShift.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MapShift::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MapShift::Start(){

}

//���t���[���Ă΂�܂�
void MapShift::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void MapShift::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MapShift::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MapShift::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void MapShift::OnCollideExit(GameObject target){
	(void)target;
}

MapShift * MapShift::GetMapShift(GameObject object)
{
	if (object) {
		return object->GetScript<MapShift>();
	}
	return nullptr;
}