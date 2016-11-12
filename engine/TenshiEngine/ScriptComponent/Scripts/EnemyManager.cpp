#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"

#include "Enemy.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){
	EnemyTeamIntoEnemyContainer(m_EnemyTeam);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Start(){

}

//���t���[���Ă΂�܂�
void EnemyManager::Update(){
	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end();) {
		//�e�������Ă��邩�ǂ���
		bool parentAlive = false;
		for (auto j = i->begin(); j != i->end();) {
			if (j->Get()->EndFinish()) {
				j = i->erase(j);
			}
			else {
				///����������
				auto jScript = j->Get()->GetScript<Enemy>();
				if (!jScript)return;
				if (jScript->GetChildFlag()) {
					if (parentAlive) {

					}
					else {

					}
				}
				else {
					//�m���ɐe���擪�ɓ����Ă���̂ł����Ő����m�F������
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
