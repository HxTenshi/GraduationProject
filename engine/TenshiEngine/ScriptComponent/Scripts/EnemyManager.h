
#pragma once
#include "main.h"
#include "Enemy.h"

struct EnemyParameter {
	//�q�����ǂ���
	bool child = true;

	//���̃A�N�V�������[�h(TRACKING or BATTLE)��ID
	ACTIONMODE nowActionMode = ACTIONMODE::TRACKINGMODE;
	
	//�{�����̃p�����[�^
	TrackingModeParameter trackingModeParameter;

	//�퓬���̃p�����[�^
	BattleModeParameter battleModeParameter;
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

	//�����Ă��邩
	bool discovery = false;

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
	GameObject m_EnemyTeam;
	SERIALIZE
	bool m_DrawFlag;

	//Enemy�̓��ꕨ��GameObject������
	void EnemyTeamIntoEnemyContainer(GameObject g);

	//
	std::vector<EnemyTeam> m_EnemyContainer;
};