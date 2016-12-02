#include "EnemyWeapon.h"
#include "Enemy.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyWeapon::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyWeapon::Start(){

}

//毎フレーム呼ばれます
void EnemyWeapon::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyWeapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyWeapon::OnCollideBegin(GameObject target){
	if (!enemy)return;
	auto enemyScript = Enemy::GetEnemy(enemy);
	if (!enemyScript)return;
	enemyScript->Attack(target);
}

//コライダーとのヒット中に呼ばれます
void EnemyWeapon::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyWeapon::OnCollideExit(GameObject target){
	(void)target;
}