#include "EnemyOneTeam.h"
#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"

#include "Game/Component/CharacterControllerComponent.h"

void EnemyOneTeam::TeamInitialize()
{
	auto enemyTeamChild = gameObject->mTransform->Children();
	if (enemyTeamChild.size() == 0) {
		return;
	}
	for (auto& i : enemyTeamChild) {
		if (!i)return;
		auto iScript = Enemy::GetEnemy(i);
		if (!iScript)return;

		EnemyOne eo;
		eo.enemyParameter.enemy_type = iScript->GetEnemyType();
		eo.enemyGameObject = i;
		teamMember.push_back(eo);
		m_DiscoveryPlayer = false;
	}
}

bool EnemyOneTeam::Alive()
{
	//チームごとに回している
	for (auto j = teamMember.begin(); j != teamMember.end();) {
		//死んだら
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return false;
		if (jScript->IsEnd()) {
			//vectorから削除
			jScript->ChildFinalize();
			j = teamMember.erase(j);
			if (teamMember.size() == 0) {
				return false;
			}
			continue;
		}
		j++;
	}

	return true;
}

void EnemyOneTeam::DiscoveryOrLostPlayerSet()
{
	for (auto j = teamMember.begin(); j != teamMember.end(); j++) {
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return;
		if (!j->enemyParameter.m_ArcharAttackStart) {
			j->enemyParameter.m_ArcharAttackStart =	jScript->DiscoveryPlayer();
		}
		else {
			j->enemyParameter.m_ArcharAttackStart = !jScript->LostPlayer();
		}
	}
}

void EnemyOneTeam::TeamUpdate()
{
	for (auto j = teamMember.begin(); j != teamMember.end(); j++) {
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return;
		jScript->SoloAction();
	}
}