#include "EnemyOneTeam.h"
#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"

void EnemyOneTeam::TeamInitialize()
{
	auto enemyTeamChild = gameObject->mTransform->Children();
	if (enemyTeamChild.size() == 0)return;
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
	//ƒ`[ƒ€‚²‚Æ‚É‰ñ‚µ‚Ä‚¢‚é
	for (auto j = teamMember.begin(); j != teamMember.end();) {
		//Ž€‚ñ‚¾‚ç
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return false;
		if (jScript->IsEnd()) {
			//vector‚©‚çíœ
			j = teamMember.erase(j);
			Hx::Debug()->Log("erase");
			Enemy::GetEnemy(j->enemyGameObject)->ChildFinalize();
			if (teamMember.size() == 0) {
				return false;
				Hx::Debug()->Log("teamDie");
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
		if (!m_DiscoveryPlayer) {
			m_DiscoveryPlayer =	jScript->DiscoveryPlayer();
		}
		else {
			m_DiscoveryPlayer = !jScript->LostPlayer();
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