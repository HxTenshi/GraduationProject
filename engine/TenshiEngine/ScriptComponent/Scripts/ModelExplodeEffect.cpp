#include "ModelExplodeEffect.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ModelExplodeEffect::Initialize(){
	if (expModel1)objects.push_back(expModel1);
	if (expModel2)objects.push_back(expModel2);
	if (expModel3)objects.push_back(expModel3);
	if (expModel4)objects.push_back(expModel4);
	if (expModel5)objects.push_back(expModel5);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ModelExplodeEffect::Start(){
	time = 0.0f;
	count_auto_delete_time = 0.0f;
	explode_flag = false;
	for (auto i : objects) {
		i->Disable();
	}
}

//毎フレーム呼ばれます
void ModelExplodeEffect::Update(){
	Timer();
	ExplodeFunc();
	AutoDeleteTimer();
	AutoDelete();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ModelExplodeEffect::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ModelExplodeEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ModelExplodeEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ModelExplodeEffect::OnCollideExit(GameObject target){
	(void)target;
}

void ModelExplodeEffect::Timer()
{
	time += Hx::DeltaTime()->GetDeltaTime();
}

void ModelExplodeEffect::ExplodeFunc()
{
	if (explode_flag)return;
	if (time >= explode_time) {
		for (auto i : objects) {
			i->Enable();
		}
		explode_flag = true;
	}
}

void ModelExplodeEffect::AutoDeleteTimer()
{
	count_auto_delete_time += Hx::DeltaTime()->GetDeltaTime();
}

void ModelExplodeEffect::AutoDelete()
{
	if (count_auto_delete_time >= auto_delete_time) {
		Hx::DestroyObject(gameObject);
	}
}
