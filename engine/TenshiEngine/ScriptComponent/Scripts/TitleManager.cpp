#include "TitleManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void TitleManager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TitleManager::Start(){

}

//毎フレーム呼ばれます
void TitleManager::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TitleManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TitleManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TitleManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TitleManager::OnCollideExit(GameObject target){
	(void)target;
}