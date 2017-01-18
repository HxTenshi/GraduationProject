#include "MapChangePoint.h"
#include "MiniMap.h"
#include "PlayerController.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MapChangePoint::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void MapChangePoint::Start(){

}

//���t���[���Ă΂�܂�
void MapChangePoint::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void MapChangePoint::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MapChangePoint::OnCollideBegin(GameObject target){
	
	if (target) {
		if (auto p = target->GetScript<PlayerController>()) {
			ChangeMiniMap();
		}
	}
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void MapChangePoint::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void MapChangePoint::OnCollideExit(GameObject target){
	(void)target;
}

void MapChangePoint::ChangeMiniMap()
{

	if (auto map = MiniMap::GetMiniMapUI()) {
		map->Change(m_MapTexture, m_MapScale, m_LU_Point, m_RD_Point);
	}

}
