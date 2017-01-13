#pragma once
#include "main.h"
#include "Enemy.h"

class EnemyEbilEye : public Enemy {
public:
	EnemyEbilEye();
	void ChildInitialize() override;
	void SoloAction() override;
	ENEMY_TYPE GetEnemyType() override;
	float GetOnBattleRange() override;
	void Attack(GameObject player) override;
	bool Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_) override;
	bool DiscoveryPlayer() override;
	bool LostPlayer() override;

private:
	//ÉÅÉìÉoïœêî
	void TrackingModeInitilize();
	void TrackingModeUpdate();
	void TrackingModeFinalize();

	void TrackingMoveModeInitilize();
	void TrackingMoveModeUpdate();
	void TrackingMoveModeFinalize();

	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	SERIALIZE XMVECTOR m_MovePositionCenter;
	SERIALIZE float m_MovePositionRadius;
	SERIALIZE float m_RotateSpeed;
	SERIALIZE float m_TrackingSpeed;

	float m_RotateAngle;
public:
};