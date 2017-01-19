#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "EnemyTeam.h"
#include "Enemy.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Start(){
	EnemysIntoEnemyContainer();
}

//毎フレーム呼ばれます
void EnemyManager::Update(){
	//敵の入れ物を全部回す
	for (auto i = m_EnemyTeamVector.begin(); i != m_EnemyTeamVector.end();) {
		auto jScript = EnemyTeam::GetEnemyTeam(*i);
		if (!jScript->Alive()) {
			Hx::DestroyObject((*i)->mTransform->gameObject);
			i = m_EnemyTeamVector.erase(i);
			continue;
		}
		jScript->DiscoveryOrLostPlayerSet();
		jScript->TeamUpdate();
		i++;
	}
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

void EnemyManager::EnemysIntoEnemyContainer(){
	if (!m_Enemys)return;
	auto enemyTeams = m_Enemys->mTransform->Children();
	if (enemyTeams.size() == 0)return;
	for (auto& newEnemyTeam : enemyTeams) {
		m_EnemyTeamVector.push_back(newEnemyTeam);
		EnemyTeam::GetEnemyTeam(newEnemyTeam)->TeamInitialize();
	}
}