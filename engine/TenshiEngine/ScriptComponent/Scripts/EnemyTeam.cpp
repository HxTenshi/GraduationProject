#include "EnemyTeam.h"
#include "EnemyRezardManTeam.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyTeam::Initialize(){
	//�N���U�����邩
	whoAttack = 0;

	//���̍U���܂ł̎���
	nextAttackTime = 0;

	//�U������T������
	everyoneAttack = false;

	//���̍U���܂ł̎��Ԃ��J�E���g����
	nextAttackTimeCount = 0;

	//��čU���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	everyoneAttackCountFlag = false;
	
	//�e�������Ă��邩�ǂ���
	parentAlive = true;

	battlePosFirst = 0;

	m_AttackStart = false;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyTeam::Start(){

}

//���t���[���Ă΂�܂�
void EnemyTeam::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyTeam::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyTeam::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyTeam::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyTeam::OnCollideExit(GameObject target){
	(void)target;
}

EnemyTeam * EnemyTeam::GetEnemyTeam(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<EnemyRezardManTeam>())return scr;

	return NULL;
}
