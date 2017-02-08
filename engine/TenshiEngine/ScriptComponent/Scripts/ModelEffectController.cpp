#include "ModelEffectController.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ModelEffectController::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ModelEffectController::Start(){
	time = 0;
	delete_time = 0;
}

//毎フレーム呼ばれます
void ModelEffectController::Update(){
	UpperFunc();
	AutoDelete();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ModelEffectController::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ModelEffectController::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ModelEffectController::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ModelEffectController::OnCollideExit(GameObject target){
	(void)target;
}

void ModelEffectController::UpperFunc()
{
	if (!isUpper)return;
	time += Hx::DeltaTime()->GetDeltaTime();
	if (OverCheck())return;
	if (start_time <= time) {
		auto pos = ModelEff->mTransform->Position();
		pos.y += Hx::DeltaTime()->GetDeltaTime()*speed;
		ModelEff->mTransform->Position(pos);
	}
}

bool ModelEffectController::OverCheck()
{
	if (ModelEff->mTransform->Position().y > up-0.1f) {
		auto pos = ModelEff->mTransform->Position();
		pos.y = up;
		ModelEff->mTransform->Position(pos);
		return true;
	}
	return false;
}

void ModelEffectController::AutoDelete()
{
	delete_time += Hx::DeltaTime()->GetDeltaTime();
	if (delete_time >= auto_delete_time) {
		ModelEff->mTransform->Disable();
	}
	if (delete_time+3 >= auto_delete_time) {
		Hx::DestroyObject(gameObject);
	}
}
