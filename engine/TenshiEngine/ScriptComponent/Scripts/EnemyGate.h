
#pragma once
#include "main.h"
#include "Enemy.h"
class EnemyGate :public Enemy {
public:
	EnemyGate();
	void ChildInitialize() override;
	void SoloAction() override;
	ENEMY_TYPE GetEnemyType() override;
	float GetOnBattleRange() override;
	void Attack(GameObject player, COL_TYPE colType) override;
	bool Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_) override;
	bool DiscoveryPlayer() override;
	bool LostPlayer() override;
	void ChildFinalize() override;


private:
	//ÉÅÉìÉoïœêî
	void TrackingModeInitilize();
	void TrackingModeUpdate();
	void TrackingModeFinalize();

	SERIALIZE float hp;
	SERIALIZE GameObject mGate;
public:
};