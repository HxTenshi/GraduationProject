
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//�q�����ǂ���
	bool child = true;

	//�U�����邩
	bool attack = false;

	//�U������T������
	bool everyoneAttack = false;

	//���̍U���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	bool nextAttackTimeCountFlag = false;
};

struct EnemyOne {
	//�X�̓G��gameObject
	GameObject enemyGameObject;

	//�X�̓G�̃p�����[�^
	EnemyParameter enemyParameter;
};

struct EnemyTeamParameter {
	//�`�[���̔ԍ�
	int teamID = 0;

	//�`�[���̐l��
	int teamCount = 0;

	//�N���U�����邩
	int whoAttack = 0;

	//���̍U���܂ł̎���
	float nextAttackTime = 0;

	//�U������T������
	bool everyoneAttack = false;

	//���̍U���܂ł̎��Ԃ��J�E���g����
	float nextAttackTimeCount = 0;

	//��čU���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	bool everyoneAttackCountFlag = false;

	//�����Ă��邩
	bool discoveryPlayer = false;

	//����������
	bool lostPlayer = false;

	//�e�������Ă��邩�ǂ���
	bool parentAlive = true;
};

struct EnemyTeam {
	//�X�̓G��gameObject�ƃp�����[�^���܂Ƃ߂����̂��`�[���������Ă����
	std::vector<EnemyOne> enemyOneVec;

	//�`�[���̃p�����[�^
	EnemyTeamParameter enemyTeamParameter;
};

class EnemyManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//�����o�ϐ�
	SERIALIZE
	GameObject m_Enemys;
	SERIALIZE
	bool m_DrawFlag;
	SERIALIZE
	GameObject m_Player;
	SERIALIZE
	float m_NextAttackTimeMin;
	SERIALIZE
	float m_NextAttackTimeMax;

	//Enemy�̓��ꕨ��GameObject������
	void EnemysIntoEnemyContainer();

	//
	std::vector<EnemyTeam> m_EnemyContainer;
};