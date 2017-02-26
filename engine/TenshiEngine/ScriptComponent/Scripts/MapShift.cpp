#include "MapShift.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void MapShift::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MapShift::Start(){

}

//毎フレーム呼ばれます
void MapShift::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void MapShift::Finish(){

}

//コライダーとのヒット時に呼ばれます
void MapShift::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void MapShift::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
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