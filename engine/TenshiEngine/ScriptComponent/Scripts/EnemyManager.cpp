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
	for (auto i : m_EnemyContainer) {
		for (auto j : i.enemyOneVec) {
			auto jScript = j.enemyGameObject->GetScript<Enemy>();
			if (!jScript)return;
			jScript->ManagerInitialize();
			if (j.enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE) {
				jScript->ActionInitilize(j.enemyParameter.trackingModeParameter.trackingActionID);
			}
			else {
				jScript->ActionInitilize(j.enemyParameter.battleModeParameter.battleActionID);
			}
		}
	}
}

//毎フレーム呼ばれます
void EnemyManager::Update(){
	if (Input::Trigger(KeyCode::Key_0))EnemyTeamIntoEnemyContainer(m_EnemyTeam);

	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end();) {
		bool deleteTeam = false;
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end();) {
			if (j->enemyGameObject.Get()->EndFinish()) {
				j = i->enemyOneVec.erase(j);
				i->enemyTeamParameter.teamCount -= 1;
				if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + std::to_string(i->enemyTeamParameter.teamCount) + "人");
				if (i->enemyOneVec.size() == 0) {
					if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + "チームが壊滅した");
					i = m_EnemyContainer.erase(i);
					deleteTeam = true;
					break;
				}
				else {
					continue;
				}
			}

			//発見したかどうか

			j++;
		}
		if (deleteTeam)continue;
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end();j++) {
			auto jScript = j->enemyGameObject.Get()->GetScript<Enemy>();
			if (!jScript)return;
			if (j->enemyParameter.child) {
				//子分の処理
				if (i->enemyTeamParameter.parentAlive) {
					//親が生きていたら
					for (auto i : m_EnemyContainer) {
						for (auto j : i.enemyOneVec) {
							if (j.enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE)
								j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.trackingModeParameter.trackingActionID);
							else
								j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.battleModeParameter.battleActionID);
						}
					}
				}
				else {
					//親が死んでいたら
					for (auto i : m_EnemyContainer) {
						for (auto j : i.enemyOneVec) {
							if (j.enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE)
								j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.trackingModeParameter.trackingActionID);
							else
								j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.battleModeParameter.battleActionID);
						}
					}
				}
			}
			else {
				//親分の処理
				for (auto i : m_EnemyContainer) {
					for (auto j : i.enemyOneVec) {
						if (j.enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE)
							j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.trackingModeParameter.trackingActionID);
						else
							j.enemyGameObject->GetScript<Enemy>()->ActionUpdate(j.enemyParameter.battleModeParameter.battleActionID);
					}
				}
			}
		}
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

void EnemyManager::EnemyTeamIntoEnemyContainer(GameObject g){
	auto newEnemyTeam = Hx::Instance(g);
	if (!newEnemyTeam)return;
	newEnemyTeam->Enable();
	EnemyOne eo;
	EnemyTeam et;
	int count = 0;
	if (newEnemyTeam->GetScript<Enemy>()) {
		count++;
		eo.enemyGameObject = newEnemyTeam;
		eo.enemyParameter.child = false;
		eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
		eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::PARENTTRACKING;
		et.enemyOneVec.insert(et.enemyOneVec.begin(), eo);
	}
	else {
		auto enemyTeamChild = newEnemyTeam->mTransform->Children();
		if (enemyTeamChild.size() == 0)return;

		for (auto i : enemyTeamChild) {
			count++;
			if (!i)return;
			auto iScript = i->GetScript<Enemy>();
			eo.enemyGameObject = i;
			if (iScript->GetChildFlag()) {
				eo.enemyParameter.child = true;
				eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
				eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::CHILDTRACKING;
				et.enemyOneVec.push_back(eo);
			}
			else {
				eo.enemyParameter.child = false;
				eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
				eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::PARENTTRACKING;
				et.enemyOneVec.insert(et.enemyOneVec.begin(), eo);
			}
		}
	}

	et.enemyTeamParameter.teamCount = count;

	count = 0;
	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end(); i++) {
		if (i->enemyTeamParameter.teamID == count) {
			count++;
			i = m_EnemyContainer.begin();
		}
	}
	et.enemyTeamParameter.teamID = count;
	if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(et.enemyTeamParameter.teamID) + " : " + std::to_string(et.enemyTeamParameter.teamCount) + "人");
	m_EnemyContainer.push_back(et);
}
