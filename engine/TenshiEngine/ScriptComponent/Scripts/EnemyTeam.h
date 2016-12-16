
#pragma once
#include "main.h"
#include "EnemyManager.h"

struct EnemyParameter {
	//�U�����邩
	bool attack = false;

	//�U������T������
	bool everyoneAttack = false;

	//���̍U���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	bool nextAttackTimeCountFlag = false;

	ENEMY_TYPE enemy_type = ENEMY_TYPE::PARENT;
};

struct EnemyOne {
	//�X�̓G��gameObject
	GameObject enemyGameObject;

	//�X�̓G�̃p�����[�^
	EnemyParameter enemyParameter;
};

class EnemyTeam :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	virtual void TeamInitialize() {}
	virtual bool Alive() { return false; };
	virtual void DiscoveryOrLostPlayerSet() {};
	virtual void TeamUpdate() {};

	static EnemyTeam* GetEnemyTeam(GameObject target);
	void SetPlayer(GameObject gameObject) { m_Player = gameObject; };
private:
	//�����o�ϐ�

protected:
	GameObject m_Player;
	
	std::vector<EnemyOne> teamMember;
	bool m_DiscoveryPlayer;
	bool wasAttacked;
	//�N���U�����邩
	int whoAttack;

	//���̍U���܂ł̎���
	float nextAttackTime;

	//�U������T������
	bool everyoneAttack;

	//���̍U���܂ł̎��Ԃ��J�E���g����
	float nextAttackTimeCount;

	//��čU���܂ł̎��Ԃ��J�E���g���邩�ǂ���
	bool everyoneAttackCountFlag;
	//�e�������Ă��邩�ǂ���
	bool parentAlive;

	int battlePosFirst;

	int archerCount = 0;

	bool m_AttackStart;
};