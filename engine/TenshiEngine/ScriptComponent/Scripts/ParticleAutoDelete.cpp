#include "ParticleAutoDelete.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ParticleAutoDelete::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ParticleAutoDelete::Start(){

}

//毎フレーム呼ばれます
void ParticleAutoDelete::Update(){
	count_auto_delete_time += Hx::DeltaTime()->GetDeltaTime();
	if (auto_delete_time <= count_auto_delete_time)Hx::DestroyObject(gameObject);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ParticleAutoDelete::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ParticleAutoDelete::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ParticleAutoDelete::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ParticleAutoDelete::OnCollideExit(GameObject target){
	(void)target;
}