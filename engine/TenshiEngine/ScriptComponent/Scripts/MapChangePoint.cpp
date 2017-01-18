#include "MapChangePoint.h"
#include "MiniMap.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void MapChangePoint::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MapChangePoint::Start(){

}

//毎フレーム呼ばれます
void MapChangePoint::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void MapChangePoint::Finish(){

}

//コライダーとのヒット時に呼ばれます
void MapChangePoint::OnCollideBegin(GameObject target){
	
	if (target) {
		if (auto p = target->GetScript<PlayerController>()) {
			ChangeMiniMap();
		}
	}
}

//コライダーとのヒット中に呼ばれます
void MapChangePoint::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void MapChangePoint::OnCollideExit(GameObject target){
	(void)target;
}

void MapChangePoint::ChangeMiniMap()
{

	if (auto map = MiniMap::GetMiniMapUI()) {
		map->Change(m_MapTexture, m_MapScale, m_LU_Point, m_RD_Point);
	}

}
