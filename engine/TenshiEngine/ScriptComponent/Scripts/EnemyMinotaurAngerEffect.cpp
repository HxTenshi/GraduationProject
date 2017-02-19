#include "EnemyMinotaurAngerEffect.h"
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyMinotaurAngerEffect::Initialize(){
	if(obj1)effect.push_back(obj1);
	if(obj2)effect.push_back(obj2);
	if(obj3)effect.push_back(obj3);
	if(obj4)effect.push_back(obj4);
	if(obj5)effect.push_back(obj5);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyMinotaurAngerEffect::Start(){

}

//毎フレーム呼ばれます
void EnemyMinotaurAngerEffect::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyMinotaurAngerEffect::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyMinotaurAngerEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyMinotaurAngerEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyMinotaurAngerEffect::OnCollideExit(GameObject target){
	(void)target;
}

void EnemyMinotaurAngerEffect::Anger()
{
	for (auto i : effect) {
		if (i->GetComponent<ParticleComponent>()) {
			auto cap = i->GetComponent<ParticleComponent>()->mParticleCapacity;
			i->GetComponent<ParticleComponent>()->mParticleParam.Param.x = cap;

		}
	}
}
