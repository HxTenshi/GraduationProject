#include "Bomb.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Bomb::Initialize(){
	started_flag = false;
	is_start = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Bomb::Start(){

}

//毎フレーム呼ばれます
void Bomb::Update(){
	//手に持っている状態で起動していなかったら
	if (m_weapon_object->GetScript<Weapon>()->GetIsHand() && !is_start) {
		is_start = true;
	}
	if (is_start) {
		CountDown();
		Explosion();
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Bomb::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Bomb::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Bomb::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Bomb::OnCollideExit(GameObject target){
	(void)target;
}

void Bomb::CountDown()
{
	if (m_explosion_time <= 0) {
		m_explosion_time = 0;
		return;
	}
	m_explosion_time -= Hx::DeltaTime()->GetDeltaTime();
}

void Bomb::Explosion()
{
	if (m_explosion_time<=0) {

		//爆発処理
		Hx::DestroyObject(m_weapon_object);
	}
}
