
#pragma once
#include "main.h"
#include <functional>
#include <vector>
#include "WeaponCommon.h"
#include "Enemy.h"

enum NextAttack {
	None,
	Low,
	High,
	Special,
};

struct AttackState {
	int ID = -1;
	int NextLowID = -1;
	int NextHighID = -1;
	int MoutionID = -1;
	float AttackTime = 0.0f;
	float KoutyokuTime = 0.0f;
	float NextTime = 0.0f;
	float DamageScale = 0.0f;
	float AddSpecial = 0.0f;
	float AttackMove = 0.0f;
	float FloatMove = 0.0f;
	std::function<void(void)> AttackFunc = []() {};
	DamageType DamageType = DamageType::LowDamage;
	BATTLEACTION::Enum KnockbackEffect = BATTLEACTION::WINCEACTION;
	float KnockbackEffectPower = 0.0f;
};

class CharacterControllerComponent;
class PlayerController :public IDllScriptComponent{
public:
	PlayerController();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	struct PlayerState {
		enum Enum {
			Lock,
			Free,
			Guard,
			Dodge,
			Attack,
			KnockBack,
			Down,
			Movie,
			Dead,
			Count
		};
	};
	struct KnockBack {
		enum Enum {
			None,
			None_NoInvisible,
			Low,
			Down,
		};
	};
	void SetPlayerState(PlayerState::Enum state);
	PlayerState::Enum GetPlayerState();
	void PlayKnockBack(const XMVECTOR& attackVect, KnockBack::Enum knockBackLevel);
	void Damage(float damage, const XMVECTOR& attackVect, KnockBack::Enum knockBackLevel = KnockBack::None);
	bool IsInvisible();
	bool IsDead();
	bool IsDogde();
	bool IsGuard();

	void SetSpecial(float power);
	void AddSpecial(float power);
	float GetSpecial();

	void SetHP(float power);
	void AddHP(float power);
	float GetHP();

	void SetMaxHP(float power);
	void AddMaxHP(float power);
	float GetMaxHP();

	void Dead();
	void AddCoroutine(const std::function<bool(void)>& func);

	int GetHitComboCount();
private:

	void LockEnter();
	void LockExcute();
	void LockExit();

	void FreeEnter();
	void FreeExcute();
	void FreeExit();

	void GuardEnter();
	void GuardExcute();
	void GuardExit();

	void AttackEnter();
	void AttackExcute();
	void AttackExit();

	void DodgeEnter();
	void DodgeExcute();
	void DodgeExit();

	void KnockBackEnter();
	void KnockBackExcute();
	void KnockBackExit();

	void DownEnter();
	void DownExcute();
	void DownExit();

	void MovieEnter();
	void MovieExcute();
	void MovieExit();

	void DeadEnter();
	void DeadExcute();
	void DeadExit();

	void move();
	void dontmove();
	void moveUpdate();
	void rotate();
	bool dodge();
	void guard();
	bool attack();
	void throwAway(GameObject target = NULL, bool isMove = false);
	void lockOn();
	void GettingWeapon();

	float GetMovementSpeed();

	void changeAnime(int id);
	float getMoutionTime(int id);
	void animeFlip();

	void stateFlip();
	//メンバ変数

	void ComboAdvantage();
	void AddCombo();
	void ClearCombo();

	PlayerState::Enum m_PlayerState_Stack;
	PlayerState::Enum m_PlayerState;
	struct StateFunc{
		std::function<void(void)> Enter = []() {};
		std::function<void(void)> Excute = []() {};
		std::function<void(void)> Exit = []() {};
	};

	std::vector<StateFunc> m_StateFunc;

	float m_InvisibleTime;
	bool m_IsInvisible;
	bool m_IsDead;
	SERIALIZE
	float m_MaxHP;
	SERIALIZE
	float m_HP;
	SERIALIZE
	float m_SpecialPower;
	const float m_SpecialPowerMax;

	SERIALIZE
	GameObject m_AnimeModel;
	SERIALIZE
	GameObject m_Camera;
	SERIALIZE
	float m_JumpPower;
	SERIALIZE
	float m_MoveSpeed;


	float m_MoveSpeed_ComboAdd;
	float m_MoutionSpeed_ComboAdd;
	float m_WeaponResist_ComboAdd;

	float m_MoutionSpeed;

	float m_MoveX;
	float m_MoveZ;

	//11 / 04追加更新
	SERIALIZE
	GameObject m_WeaponHand;
	SERIALIZE
	GameObject m_GetEnemy;
	SERIALIZE
	GameObject m_GetWeapon;
	SERIALIZE
	GameObject m_marker;
	SERIALIZE
	GameObject m_TimeManager;
	GameObject m_tempWeapon;
	float m_InputF_Time;	//Fキー何秒押されたか

	SERIALIZE 
	GameObject mMoveAvility;
	SERIALIZE 
	GameObject mAimController;
	SERIALIZE
	GameObject mWeaponControl;

	SERIALIZE
	PrefabAsset m_BloodParticle;
	SERIALIZE
	PrefabAsset m_GuardParticle;
	SERIALIZE
	GameObject m_soundManager;

	float m_FloatJumpTimer;

	int m_HitCount;
	float m_ComboTimer;

	XMVECTOR m_MoveVelo;
	XMVECTOR mJump;
	XMVECTOR mGravity;
	//向くべき方向
	XMVECTOR mVelocity;

	struct DogdeParam {
		XMVECTOR Velocity = XMVectorZero();
		float Timer = 0.0f;
	} m_DogdeParam;

	float m_RotateLimit;
	struct GuardParam{
		bool AttackGuard = false;
		float KnockBackTime = 0.0f;
		float JustTime = 0.0f;
		float JustTimeCooldomn = 0.0f;
	} m_GuardParam;
	bool m_IsGround;
	bool m_IsGuard;

	bool m_LockOnEnabled;

	int m_CurrentAnimeID_Stack;
	int m_CurrentAnimeID;

	weak_ptr<CharacterControllerComponent> m_CharacterControllerComponent;

	bool m_AttackMode;
	int m_NextAttack;
	AttackState m_CurrentAttack;
	int m_CurrentWeaponType;
	std::vector<std::vector<AttackState>> m_AttackStateList;


	std::list<std::function<bool(void)>> m_UpdateCoroutine;
};