#include "CreditDraw.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void CreditDraw::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreditDraw::Start(){

}

//毎フレーム呼ばれます
void CreditDraw::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreditDraw::Finish(){

}

//コライダーとのヒット時に呼ばれます
void CreditDraw::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CreditDraw::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CreditDraw::OnCollideExit(GameObject target){
	(void)target;
}

TextureAsset CreditDraw::GetTexture()
{
	return m_texture;
}