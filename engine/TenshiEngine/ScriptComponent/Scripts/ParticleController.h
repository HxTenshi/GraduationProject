
#pragma once
#include "main.h"
#include <vector>

class ParticleController :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;


	void ParticleNumSwitch(bool flag);
	void NumFunc();
	void NumDeleteTimer();

	void AutoDeleteFunc();
	void AutoDeleteTimer();


private:

	SERIALIZE int m_particle_count;
	SERIALIZE bool m_start_num_delete_flag;
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_Particle01;
	SERIALIZE GameObject m_Particle02;
	SERIALIZE GameObject m_Particle03;
	SERIALIZE GameObject m_Particle04;
	SERIALIZE GameObject m_Particle05;
	std::vector<GameObject> m_Particle;
	SERIALIZE bool num_delete_flag;
	SERIALIZE float num_delete_time;
	SERIALIZE bool num_once_flag;
	float count_num_delete_time;

	int num_delete_count;

	//auto delete


	SERIALIZE bool auto_delete_flag;
	SERIALIZE float auto_delete_time;
	float count_auto_delete_time;
};