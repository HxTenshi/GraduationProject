#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "EnemyTeam.h"
#include "Enemy.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Start(){
	EnemysIntoEnemyContainer();
}

//���t���[���Ă΂�܂�
void EnemyManager::Update(){
	//�G�̓��ꕨ��S����
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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