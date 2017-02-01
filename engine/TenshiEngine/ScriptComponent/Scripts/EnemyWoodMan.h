#pragma once
#include "main.h"
#include "Enemy.h"

class EnemyWoodMan : public Enemy {
public:
	EnemyWoodMan();
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
		ANIM_RUNATTACK,
		ANIM_TIRED,
		ANIM_DOWN,
		ANIM_NOMALATTACK,
		ANIM_IDLE
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

	void RunAttackModeInitilize();
	void RunAttackModeUpdate();
	void RunAttackModeFinalize();

	void ComeBackModeInitilize();
	void ComeBackModeUpdate();
	void ComeBackModeFinalize();

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

	void LookPosition(XMVECTOR position_);

	SERIALIZE XMVECTOR m_MovePositionCenter;
	SERIALIZE float m_MovePositionRadius;
	SERIALIZE float m_MoveRotateSpeed;
	SERIALIZE float m_Speed;
	SERIALIZE float m_TrackingRange;
	SERIALIZE float m_TrackingAngle;
	SERIALIZE float m_LostRange;
	SERIALIZE float m_RotateSpeed;
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE float m_TackleSpeed;
	SERIALIZE float m_TackleTime;
	SERIALIZE float m_TackleStartRandMin;
	SERIALIZE float m_TackleStartRandMax;
	SERIALIZE float m_WinceTime;
	SERIALIZE float hp;

	float m_RotateAngle;
	float m_Count;
	bool m_RotateStart;
	bool m_RotateEnd;
	XMVECTOR m_TackleVec;
	XMVECTOR m_TackleStartPos;
	float m_WinceCount;
	bool m_UpperdownNow;
	bool m_DeadIsGround;

	XMVECTOR m_StartForward;
	XMVECTOR m_StartPos;

	BATTLEACTION::Enum m_WinceBeforeId;
	bool m_Attacked;
public:
};