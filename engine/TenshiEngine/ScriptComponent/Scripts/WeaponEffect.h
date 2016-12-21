
#pragma once
#include "main.h"


class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//�A�N�V�����̋N������1��ň����
	void Action();
	//�A�j���[�V�����̈Ⴂ�ɂ��^�C���̃Z�b�g
	void SetLifeTime(float time,float dtime=0.5f);

private:
	//�����o�ϐ�
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	GameObject m_now_Effect;
	SERIALIZE
	float m_life_time;
	SERIALIZE
	float m_stop_particle_time;


	

};