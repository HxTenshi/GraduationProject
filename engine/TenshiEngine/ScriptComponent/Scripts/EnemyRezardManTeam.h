
#pragma once
#include "main.h"
#include "EnemyTeam.h"


class EnemyRezardManTeam :public EnemyTeam{
public:
	void TeamInitialize()override;
	bool Alive() override;
	void DiscoveryOrLostPlayerSet() override;
	void TeamUpdate() override;

private:
	SERIALIZE 
		GameObject m_Player;
	SERIALIZE 
		bool m_DrawFlag;
	SERIALIZE
		float m_NextAttackTimeMin;
	SERIALIZE
		float m_NextAttackTimeMax;

};