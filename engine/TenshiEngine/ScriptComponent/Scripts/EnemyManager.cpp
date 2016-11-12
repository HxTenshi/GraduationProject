#include "EnemyManager.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){
	EnemyTeamIntoEnemyContainer(m_EnemyTeam);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Start(){

}

//���t���[���Ă΂�܂�
void EnemyManager::Update(){

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

