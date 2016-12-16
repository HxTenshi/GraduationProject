
#pragma once
#include "main.h"


class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//�A�N�V�����̋N������
	void Action();
	//�A�N�V�����̎��s���̏���
	void Play();
	//���C�t�^�C���̃��Z�b�g
	void Reset();
	//���݂��Ă��邩
	bool isAlive();
	//���Ԃ��߂���ƍ폜����
	void DeleteAction();

private:
	//�����o�ϐ�
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	GameObject m_now_Effect;
	SERIALIZE
	float m_life_time;
	float m_now_life_time;
	

};