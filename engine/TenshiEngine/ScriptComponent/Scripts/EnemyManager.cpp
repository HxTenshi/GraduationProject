#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"

#include "Enemy.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){
	EnemyTeamIntoEnemyContainer(m_EnemyTeam);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Start(){

}

//毎フレーム呼ばれます
void EnemyManager::Update(){
	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end();) {
		//親が生きているかどうか
		bool parentAlive = false;
		for (auto j = i->begin(); j != i->end();) {
			if (j->Get()->EndFinish()) {
				j = i->erase(j);
			}
			else {
				///処理を書く
				auto jScript = j->Get()->GetScript<Enemy>();
				if (!jScript)return;
				if (jScript->GetChildFlag()) {
					if (parentAlive) {

					}
					else {

					}
				}
				else {
					//確実に親が先頭に入っているのでここで生存確認をする
					parentAlive = true;

				}
				j++;
			}
		}
		if (i->size() == 0) {
			i = m_EnemyContainer.erase(i);
		}
		else {
			i++;
		}
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

void EnemyManager::EnemyTeamIntoEnemyContainer(GameObject g){
	auto newEnemyTeam = Hx::Instance(g);
	if (!newEnemyTeam)return;
	newEnemyTeam->Enable();
	auto enemyTeamChild = newEnemyTeam->mTransform->Children();
	if (enemyTeamChild.size() == 0)return;
	std::vector<GameObject> newEnemyTeamChildVec;
	for (auto i : newEnemyTeam->mTransform->Children()) {
		if (!i)return;
		auto iScript = i->GetScript<Enemy>();
		if (iScript->GetChildFlag()) {
			newEnemyTeamChildVec.push_back(i);
		}
		else{
			newEnemyTeamChildVec.insert(newEnemyTeamChildVec.begin(), i);
		}
	}
	m_EnemyContainer.push_back(newEnemyTeamChildVec);
}
