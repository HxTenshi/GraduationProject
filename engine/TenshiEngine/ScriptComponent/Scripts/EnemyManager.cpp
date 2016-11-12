#include "EnemyManager.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){
	EnemyTeamIntoEnemyContainer(m_EnemyTeam);
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

void EnemyManager::EnemyTeamIntoEnemyContainer(GameObject g)
{
	auto newEnemyTeam = Hx::Instance(g);
	if (!newEnemyTeam)return;
	newEnemyTeam->Enable();
	auto enemyTeamChild = newEnemyTeam->mTransform->Children();
	if (enemyTeamChild.size() == 0)return;
	std::vector<GameObject> newEnemyTeamChildVec;
	for (auto i : newEnemyTeam->mTransform->Children()) {
		if (!i)return;
		newEnemyTeamChildVec.push_back(i);
	}
	m_EnemyContainer.push_back(newEnemyTeamChildVec);
}

