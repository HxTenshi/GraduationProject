#include "EnemyTeam.h"
#include "EnemyRezardManTeam.h"
#include "EnemyOneTeam.h"
#include "UniqueObject.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyTeam::Initialize(){
	//誰が攻撃するか
	whoAttack = 0;

	//次の攻撃までの時間
	nextAttackTime = 0;

	//攻撃が一週したか
	everyoneAttack = false;

	//次の攻撃までの時間をカウントする
	nextAttackTimeCount = 0;

	//一斉攻撃までの時間をカウントするかどうか
	everyoneAttackCountFlag = false;
	
	//親が生きているかどうか
	parentAlive = true;

	battlePosFirst = 0;

	TeamInitialize();

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyTeam::Start(){
	m_Player = UniqueObject::GetPlayer();
}

//毎フレーム呼ばれます
void EnemyTeam::Update(){
	if (!Alive()) {
		Hx::DestroyObject(gameObject);
	}
	DiscoveryOrLostPlayerSet();
	TeamUpdate();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyTeam::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyTeam::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyTeam::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyTeam::OnCollideExit(GameObject target){
	(void)target;
}

EnemyTeam * EnemyTeam::GetEnemyTeam(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<EnemyRezardManTeam>())return scr;

	if (auto scr = target->GetScript<EnemyOneTeam>())return scr;

	return NULL;
}
