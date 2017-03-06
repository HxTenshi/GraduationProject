#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "EnemyTeam.h"
#include "Enemy.h"

std::vector<GameObject> EnemyManager::m_Enemys;

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){
	EnemyManager::m_Enemys.clear();
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
