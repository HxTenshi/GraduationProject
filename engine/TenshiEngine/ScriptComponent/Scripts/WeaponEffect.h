
#pragma once
#include "main.h"

enum WeaponEffectType{
	STRONGEF,
	THROWEF
};
class WeaponEffect :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//�A�N�V�����̋N������1��ň����
	void Action(WeaponEffectType type = WeaponEffectType::STRONGEF);
	void DeleteParticle(float time, float dtime);
	//������ꍇ�͒������Ԃ��Z�b�g���Ďh�������Ƃ��ɂ�����x�^�C�����Z�b�g����
	//�A�j���[�V�����̈Ⴂ�ɂ��^�C���̃Z�b�g
	void SetLifeTime(float time,float dtime=0.5f);
private:
	//�����o�ϐ�
	SERIALIZE
	PrefabAsset m_particle;
	SERIALIZE
	PrefabAsset m_throw_particle;
	SERIALIZE
	GameObject weaponTip;
	SERIALIZE
	float m_life_time;
	SERIALIZE
	float m_stop_particle_time;

	GameObject m_nowParticle;

	

};