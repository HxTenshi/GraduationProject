
#pragma once
#include "main.h"
#include "Enemy.h"

class EmptyEnemy :public Enemy{
public:
	EmptyEnemy();
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
	enum ANIM_ID {
		ANIM_MOVE,
		ANIM_ROTATEATTACK,
		ANIM_ROTATEEYE,
		ANIM_TACKLE,
		ANIM_WINCE,
		ANIM_DOWN
	};
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

	void TackleModeInitilize();
	void TackleModeUpdate();
	void TackleModeFinalize();

	void RotateTackleModeInitilize();
	void RotateTackleModeUpdate();
	void RotateTackleModeFinalize();

	void WinceModeInitilize();
	void WinceModeUpdate();
	void WinceModeFinalize();

	void UpperDownInitilize();
	void UpperDownUpdate();
	void UpperDownFinalize();

	void BeatDownInitilize();
	void BeatDownUpdate();
	void BeatDownFinalize();

	void DownInitilize();
	void DownUpdate();
	void DownFinalize();

	void DeadInitilize();
	void DeadUpdate();
	void DeadFinalize();

	SERIALIZE float hp;

	SERIALIZE bool m_objective_flag;


public:
};