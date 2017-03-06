#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "EnemyTeam.h"
#include "Enemy.h"

std::vector<GameObject> EnemyManager::m_Enemys;

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){
	EnemyManager::m_Enemys.clear();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Start(){
}

//毎フレーム呼ばれます
void EnemyManager::Update(){
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyManager::OnCollideExit(GameObject target){
	(void)target;
}

void EnemyManager::EnemyPush(GameObject gameObject_)
{
	EnemyManager::m_Enemys.push_back(gameObject_);
}

void EnemyManager::EnemyErase(GameObject gameObject_)
{
	for (auto i = EnemyManager::m_Enemys.begin(); i != EnemyManager::m_Enemys.end(); i++) {
		if (i->lock() == gameObject_) {
			EnemyManager::m_Enemys.erase(i);
			break;
		}
	}
}

std::vector<GameObject> EnemyManager::GetEnemy()
{
	return EnemyManager::m_Enemys;
}

void EnemyManager::EnemyAllDelete()
{
	for (auto i : EnemyManager::m_Enemys) {
		Enemy::GetEnemy(i)->ChildNotCountFinalize();
	}
	EnemyManager::m_Enemys.clear();
}
