#pragma once
#include "main.h"
#include "Enemy.h"
# include "h_standard.h"

class EnemyOrc : public Enemy {
public:
	EnemyOrc();
	void ChildInitialize() override;
	void SoloAction() override;
	ENEMY_TYPE GetEnemyType() override;
	float GetOnBattleRange() override;
	void Attack(GameObject player, COL_TYPE colType) override;
	bool Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_) override;
	bool DiscoveryPlayer() override;
	bool LostPlayer() override;
	void ChildFinalize() override;

	void SetMovePoint(GameObject target);// { Hx::Debug()->Log("セットした!!!"); m_MovePoints = target; if (m_MovePoints)Hx::Debug()->Log(m_MovePoints->Name()); }
	void MoveFrontStart(float time);
private:
	void MoveFront();
	float m_MoveFrontCounter;
	float m_MoveFrontTime;
	enum ANIM_ID {
		ANIM_MOVE,
		ANIM_RUNATTACK,
		ANIM_WINCE,
		ANIM_NOMALATTACK,
		ANIM_IDLE,
		ANIM_DOWN
	};
	//メンバ変数
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

	void WeakAttackModeInitilize();
	void WeakAttackModeUpdate();
	void WeakAttackModeFinalize();

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

	SERIALIZE float m_MoveRotateSpeed;
	SERIALIZE float m_Speed;
	SERIALIZE float m_TrackingRange;
	SERIALIZE float m_TrackingAngle;
	SERIALIZE float m_LostRange;
	SERIALIZE float m_RotateSpeed;
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE float m_TrackingSpeed;
	SERIALIZE float m_TrackingRotateSpeed;
	SERIALIZE float m_TackleSpeed;
	SERIALIZE float m_TackleTime;
	SERIALIZE float m_TackleStartRandMin;
	SERIALIZE float m_TackleStartRandMax;
	SERIALIZE float m_AttackDamage;
	SERIALIZE float m_WinceTime;
	SERIALIZE float hp;
	SERIALIZE GameObject m_MovePoints;
	SERIALIZE float m_BattleRange;

	float m_RotateAngle;
	float m_Count;
	bool m_RotateStart;
	bool m_RotateEnd;
	XMVECTOR m_TackleVec;
	XMVECTOR m_TackleStartPos;
	float m_WinceCount;
	bool m_UpperdownNow;
	bool m_DeadIsGround;

	bool m_SetPoint;

	XMVECTOR m_StartForward;
	XMVECTOR m_StartPos;

	BATTLEACTION::Enum m_WinceBeforeId;
public:
};