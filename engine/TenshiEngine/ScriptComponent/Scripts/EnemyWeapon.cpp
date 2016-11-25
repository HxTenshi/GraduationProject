#include "EnemyWeapon.h"
#include "Enemy.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyWeapon::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyWeapon::Start(){

}

//���t���[���Ă΂�܂�
void EnemyWeapon::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyWeapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyWeapon::OnCollideBegin(GameObject target){
	if (!enemy)return;
	auto enemyScript = Enemy::GetEnemy(enemy);
	if (!enemyScript)return;
	enemyScript->Attack(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyWeapon::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyWeapon::OnCollideExit(GameObject target){
	(void)target;
}