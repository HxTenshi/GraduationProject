#include "PlayerController.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "WeaponHand.h"
#include "MoveAbility.h"
#include "TPSCamera.h"
#include "GetWeapon.h"
#include "TimeManager.h"
#include "GetEnemy.h"
#include "GetWeapon.h"
#include "GetMapShift.h"
#include "AimController.h"
#include "SoundManager.h"
#include "WeaponControl.h"
#include "SoundManager.h"
#include "OutputGimic.h"

namespace Init {
	static const float RotateLimit_default = 360.0f * 3.0f;
	static const float RotateLimit_attack = 360.0f * 0.5f;
	static const float RotateLimit_guard = 360.0f * 0.5f;
	static const float RotateLimit_dodge = 360.0f * 1000.0f;
}

struct AttackID {
	enum Enum {
		Low1,
		Low2,
		Low3,
		Low1End,
		Low2End,
		High1,
		High1End,
		High2,
		High2End,
		IdleFloatLow,
		HighFloatLow,
		FloatLow1,
		FloatLow1End,
		FloatLow2,

		IdleFloatHigh,
		HighFloatHigh,
		FloatLow1FloatHigh,
		FloatLow2FloatHigh,
		FloatHigh1,
		FloatHigh1Fall,
		FloatHigh1End,
		Special,
		DogdeAttack,
		SpeedJumpAttack,
		Count,
	};
};

struct AnimeID {
	enum Enum {
		Idle,
		Move,
		Guard_Knockback,
		Guard,
		Dogde,
		KnockBack,
		Down,
		AttackLow1,
		AttackLow2,
		AttackLow3,
		AttackSpeedJumpEnd,
		AttackHigh2,
		AttackLow1End,
		AttackLow2End,
		AttackHigh1End,
		Jump,
		SpeedJump,
		Fall,
		FallGround,
		FreeAIM,
		Throw,
		AttackSP,
		AttackHigh1,
		AttackFloatLow1,
		AttackFloatLow2,
		AttackFloatHigh,
		HighToFloatLow,
		IdleToFloatLow,
		HighToFloatHigh,
		IdleToFloatHigh,
		FloatLow1ToFloatHigh,
		FloatLow2ToFloatHigh,
		AttackHigh2End,
		AttackFloatLow1End,

		AttackRLow,
		AttackRHigh,
		AttackRSP,
		RIdle,
		RMove,
		RDodge,
		DownUp,
		AttackSpeedJump,
		AttackDodgeRoll,
		WeaponDrop,

		AttackFloatHighFall,
		AttackFloatHighEnd,

		Count,
	};
};

struct PlayerInput {
	enum Enum {
		Move_F,
		Move_D,
		Move_L,
		Move_R,
		ATK_L,
		ATK_H,
		ATK_S,
		Jump,
		Step,
		Guard,
		LockOn,
		LockOn_L,
		LockOn_R,
		GetWeaponDown,
		GetWeapon,
		//ReleaseWeapon,
		ThrowWeapon,
		FreeAIM,
		GoingWeapon,
		Count,
	};
};

static const std::function<bool(void)> g_playerInputs[PlayerInput::Count] = {

	[]()->bool { return Input::Down(KeyCode::Key_W) != 0; },//Move_F,
	[]()->bool { return Input::Down(KeyCode::Key_S) != 0; },//Move_D,
	[]()->bool { return Input::Down(KeyCode::Key_A) != 0; },//Move_L,
	[]()->bool { return Input::Down(KeyCode::Key_D) != 0; },//Move_R,

	[]()->bool { return (Input::Down(KeyCode::Key_V) == 0 && Input::Up(KeyCode::Key_C)) || Input::Trigger(PAD_X_KeyCode::Button_X); },//ATK_L,

	[]()->bool { return (Input::Down(KeyCode::Key_C) == 0 && Input::Up(KeyCode::Key_V)) || Input::Trigger(PAD_X_KeyCode::Button_Y); },//ATK_H,
	[]()->bool { return 
	(Input::Down(KeyCode::Key_C) != 0.0f &&
	Input::Down(KeyCode::Key_V) != 0.0f &&
	Input::Down(KeyCode::Key_C) <= 0.1f * (1.0f / Hx::DeltaTime()->GetNoScaleDeltaTime()) &&
	Input::Down(KeyCode::Key_V) <= 0.1f * (1.0f / Hx::DeltaTime()->GetNoScaleDeltaTime())) ||
	Input::Trigger(KeyCode::Key_Z)
	||
	Input::Trigger(PAD_X_KeyCode::Button_B)
	; },//ATK_S,
	
	[]()->bool { return Input::Trigger(KeyCode::Key_SPACE) != 0 || Input::Trigger(PAD_X_KeyCode::Button_A) != 0; },//Jump,
	[]()->bool { return Input::Trigger(KeyCode::Key_LSHIFT) != 0 || Input::Trigger(PAD_X_KeyCode::Button_R2); },//Step,
	[]()->bool { return Input::Down(KeyCode::Key_E) != 0 || Input::Down(PAD_X_KeyCode::Button_L2) != 0; },//Guard,
	[]()->bool { return Input::Trigger(KeyCode::Key_R) != 0 || Input::Trigger(PAD_X_KeyCode::Button_R3) != 0; },//LockOn,
	[]()->bool { return Input::Trigger(KeyCode::Key_LEFT) != 0 || Input::Analog(PAD_X_LevelCode::Level_RStickX).x < -0.1f; },//LockOn_L,
	[]()->bool { return Input::Trigger(KeyCode::Key_RIGHT) != 0 || Input::Analog(PAD_X_LevelCode::Level_RStickX).x > 0.1f; },//LockOn_R,
	[]()->bool { return Input::Down(KeyCode::Key_F) != 0 || Input::Down(PAD_X_KeyCode::Button_R1) != 0; },//GetWeaponDown,
	[]()->bool { return Input::Up(KeyCode::Key_F) != 0 || Input::Up(PAD_X_KeyCode::Button_R1) != 0; },//GetWeapon,
	//[]()->bool { return Input::Trigger(KeyCode::Key_E) != 0 || (Input::Down(PAD_X_KeyCode::Button_R2) != 0 && Input::Trigger(PAD_X_KeyCode::Button_X) != 0); },//ReleaseWeapon,
	[]()->bool { return Input::Up(KeyCode::Key_Q) != 0 || Input::Up(PAD_X_KeyCode::Button_L1) != 0; },//ThrowWeapon,
	[]()->bool { return Input::Down(KeyCode::Key_Q) >= 0.3f * (1.0f / Hx::DeltaTime()->GetNoScaleDeltaTime()) || Input::Down(PAD_X_KeyCode::Button_L1) >= 0.3f * (1.0f / Hx::DeltaTime()->GetNoScaleDeltaTime()); },//FreeAIM,
	[]()->bool { return Input::Trigger(KeyCode::Key_Q) != 0 || Input::Trigger(PAD_X_KeyCode::Button_L1) != 0; },//GoingWeapon,
};

inline bool BindInput(PlayerInput::Enum input) {
	return g_playerInputs[input]();
}

PlayerController::PlayerController()
	:m_SpecialPowerMax(100.0f)
	, m_SpecialPower(0.0f)
{
}

//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerController::Initialize(){
	m_InputF_Time = 0.0f;
	m_FloatJumpTimer = 0.0f;
	mJump = XMVectorZero();
	mGravity = XMVectorSet(0, -9.81f, 0,1);
	m_UseGravity = true;
	mVelocity = XMVectorZero();
	m_IsGround = false;
	m_NextAttack = -1;
	m_LockOnEnabled = false;
	m_InvisibleTime = 0.0f;
	m_IsInvisible = false;
	m_IsSlopeLimited = false;
	m_IsDead = false;
	m_IsGuard = false;
	m_RotateLimit = Init::RotateLimit_default;
	m_CurrentWeaponType = 0;
	m_ChangeAnime = false;
	m_FreeAIMMode = false;
	m_MapShiftFlag = false;
	m_GoingWeapon = false;
	m_DownUp = false;
	m_BoneBackPos = XMVectorZero();
	m_GameOverTime = 0.0f;
	m_MoveSETimer = 0.0f;

	m_JumpThrowWeapon=false;
	m_JumpLowAttack = false;
	m_JumpHighAttack = false;

	m_PlayerStateLock = false;

	m_HitCount = 0;
	m_MoveSpeed_ComboAdd = 0.0f;
	m_MoutionSpeed_ComboAdd = 0.0f;
	m_WeaponResist_ComboAdd = 0.0f;
	m_ATK_ComboAdd = 0.0f;

	m_MoutionSpeed = 1.0f;

	m_MoveX = 0.0f;
	m_MoveZ = 0.0f;

	m_MoveVelo = XMVectorZero();

	m_AttackCoolDownTimer = 0.0f;

	m_LockActions.resize(LockAction::Count, false);

	m_StateFunc.resize(PlayerState::Count);

#define AddFunc(x) \
	m_StateFunc[PlayerState::x].Enter	= std::bind(&PlayerController::x##Enter, this);\
	m_StateFunc[PlayerState::x].Excute	= std::bind(&PlayerController::x##Excute, this);\
	m_StateFunc[PlayerState::x].Exit	= std::bind(&PlayerController::x##Exit, this);

	AddFunc(Lock);
	AddFunc(Free);
	AddFunc(Guard);
	AddFunc(Dodge);
	AddFunc(SpeedJump);
	AddFunc(Attack);
	AddFunc(Special);
	AddFunc(KnockBack);
	AddFunc(Down);
	AddFunc(Movie);
	AddFunc(Dead);

	m_PlayerState = (PlayerState::Enum)-1;
	m_PlayerState_Stack = (PlayerState::Enum)-1;
	SetPlayerState(PlayerState::Lock);
	stateFlip();

	m_CurrentAnimeID = -1;
	m_CurrentAnimeID_Back = -1;
	m_CurrentAnime_Weight = 1.0f;

	
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerController::Start(){

	m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();

}


void PlayerController::SetLockAction(LockAction::Enum action, bool lock)
{
	if(action>=0&& action<LockAction::Count)
	m_LockActions[action] = lock;
}

bool PlayerController::GetLockAction(LockAction::Enum action)
{
	if (action >= 0 && action<LockAction::Count)
		return m_LockActions[action];
	return false;
}

void PlayerController::AttackInitialize()
{
	m_AttackStateList.resize(WeaponType::Count);
	for (auto& list : m_AttackStateList) {
		list.resize(AttackID::Count);
	}

	//+++++++++++++++++++++++
	m_ThrowAttack.AddSpecial = 1.0f;
	m_ThrowAttack.KnockbackEffect = BATTLEACTION::WINCEACTION;
	m_ThrowAttack.KnockbackEffectPower = 1.0f;
	m_ThrowAttack.DamageScale = 0.5f;
	m_ThrowAttack.DamageType = DamageType::HighDamage;
	{
		AttackState attack;
		auto& attacklist = m_AttackStateList[WeaponType::Sword];
		//+++++++++++++++++++++++
		attack.ID = AttackID::Low1;
		attack.NextLowID = AttackID::Low2;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackLow1;
		attack.EndID = AttackID::Low1End;
		attack.AddSpecial = 2.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.DamageScale = 1.0f;
		attack.AttackTime = 9.0f / 30.0f;//getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9.0f / 30.0f;
		attack.AttackMove = 0.0f;
		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L1;

		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::Low2;
		attack.NextLowID = AttackID::Low3;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackLow1;
		attack.EndID = AttackID::Low2End;

		attack.DamageScale = 1.2f;
		attack.AttackTime = 14.0f / 30.0f;//getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 6.0f / 30.0f;
		attack.OnDamageEnd = 14.0f / 30.0f;
		attack.AttackMove = 0.0f;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L2;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::Low3;
		attack.NextLowID = -1;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackLow1;
		attack.EndID = -1;

		attack.DamageScale = 1.5f;
		attack.AttackTime = 28.0f / 30.0f;//getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 5.0f / 30.0f;
		attack.OnDamageEnd = 18.0f / 30.0f;
		attack.AttackMove = 0.0f;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L3;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::DogdeAttack;
		attack.NextLowID = AttackID::Low2;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackDodgeRoll;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;

		attack.DamageScale = 1.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_DL;

		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::SpeedJumpAttack;
		attack.NextLowID = AttackID::IdleFloatLow;
		attack.NextHighID = AttackID::IdleFloatHigh;
		attack.MoutionID = AnimeID::AttackSpeedJump;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;
		attack.UseGravity = false;

		attack.DamageScale = 1.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_SJL;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High1;
		attack.NextLowID = -1;
		attack.NextHighID = AttackID::High2;
		attack.EndID = AttackID::High1End;
		attack.MoutionID = AnimeID::AttackHigh1;
		attack.DamageType = DamageType::HighDamage;
		attack.AttackTime = 16.0f / 30.0f;
		//attack.OnDamageStart = 6.0f / 60.0f;
		attack.Rotate = true;
		attack.UseGravity = false;

		attack.OnDamageStart = 6.0f / 30.0f;
		attack.OnDamageEnd = 16.0f / 30.0f;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_H1;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High2;
		attack.NextLowID = AttackID::HighFloatLow;
		attack.NextHighID = AttackID::HighFloatHigh;
		attack.MoutionID = AnimeID::AttackHigh1;
		attack.EndID = AttackID::High2End;
		attack.AttackTime = 23.0f / 30.0f;

		//attack.KnockbackEffect = BATTLEACTION::UPPERDOWNACTION;
		attack.KnockbackEffectPower = 10.0f;

		attack.OnDamageStart = 8.0f / 30.0f;
		attack.OnDamageEnd = 23.0f / 30.0f;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;

		attack.UseGravity = false;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_H2;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1;
		attack.NextLowID = AttackID::FloatLow2;
		attack.NextHighID = AttackID::FloatLow1FloatHigh;
		attack.MoutionID = AnimeID::AttackFloatLow1;
		attack.EndID = AttackID::FloatLow1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 11.0f / 30.0f;

		attack.UseGravity = false;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FL1;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::LowDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow2;
		attack.NextLowID = -1;
		attack.NextHighID = AttackID::FloatLow2FloatHigh;
		attack.MoutionID = AnimeID::AttackFloatLow2;
		attack.EndID = -1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 13.0f / 30.0f;

		attack.UseGravity = false;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FL2;

		attack.DamageScale = 1.5f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::LowDamage;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHigh;
		attack.EndID = AttackID::FloatHigh1Fall;
		attack.AttackTime = getMoutionTime(attack.MoutionID);


		//attack.KnockbackEffect = BATTLEACTION::BEATDOWNACTION;
		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;


		attack.UseGravity = false;


		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1Fall;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHighFall;
		attack.EndID = AttackID::FloatHigh1Fall;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;

		attack.SEID = -1;

		attack.UseGravity = true;


		//attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1End;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHighEnd;
		attack.EndID = -1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 0.0f / 30.0f;


		attack.UseGravity = true;


		//attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::Special;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackSP;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;

		attack.UseGravity = true;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_PlaySP;

		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.DamageScale = 10.0f;
		attack.AttackMove = 0.0f;
		attack.AddSpecial = 0.0f;
		attack.DamageType = DamageType::DethBrowDamage;
		attack.Rotate = false;
		attacklist[attack.ID] = attack;



		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatLow;
		attack.MoutionID = AnimeID::IdleToFloatLow;
		attack.EndID = AttackID::FloatLow1;
		attack.DamageScale = 0.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.DamageType = DamageType::LowDamage;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.Rotate = true;
		attack.UseGravity = false;
		attack.SEID = -1;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::HighFloatLow;
		attack.MoutionID = AnimeID::HighToFloatLow;
		attack.EndID = AttackID::FloatLow1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatHigh;
		attack.MoutionID = AnimeID::IdleToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::HighFloatHigh;
		attack.MoutionID = AnimeID::HighToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1FloatHigh;
		attack.MoutionID = AnimeID::FloatLow1ToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow2FloatHigh;
		attack.MoutionID = AnimeID::FloatLow2ToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;


		//+++++++++++++++++++++++
		attack.ID = AttackID::Low1End;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackLow1End;
		attack.EndID = -1;
		attack.AddSpecial = 0.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 0.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 0.0f / 30.0f;
		attack.FreeDogde = true;
		attack.UseGravity = true;

		attack.DamageScale = 0.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.AttackMove = 0.0f;
		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++

		attack.ID = AttackID::Low2End;
		attack.MoutionID = AnimeID::AttackLow2End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High1End;
		attack.MoutionID = AnimeID::AttackHigh1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High2End;
		attack.MoutionID = AnimeID::AttackHigh2End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++

		attack.ID = AttackID::FloatLow1End;
		attack.MoutionID = AnimeID::AttackFloatLow1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;

	}
	{
		AttackState attack;
		auto& attacklist = m_AttackStateList[WeaponType::Rance];

		attack.ID = AttackID::Low1;
		attack.NextLowID = AttackID::Low2;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackRLow;
		attack.AddSpecial = 2.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 5.0f;

		attack.DamageScale = 1.0f;
		attack.AttackTime = 17.0f / 30.0f;
		attack.AttackMove = 0.0f;
		attack.FloatMove = 0.0f;

		attack.OnDamageStart = 8.0f / 30.0f;
		attack.OnDamageEnd = 17.0f / 30.0f;

		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L1;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::Low2;
		attack.NextLowID = AttackID::Low3;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackRLow;

		attack.DamageScale = 1.1f;
		attack.AttackTime = 14.0f / 30.0f;

		attack.OnDamageStart = 6.0f / 30.0f;
		attack.OnDamageEnd = 14.0f / 30.0f;

		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L2;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::Low3;
		attack.NextLowID = -1;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackRLow;

		attack.DamageScale = 1.2f;
		attack.AttackTime = 21.0f / 30.0f;

		attack.OnDamageStart = 10.0f / 30.0f;
		attack.OnDamageEnd = 16.0f / 30.0f;

		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L3;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::DogdeAttack;
		attack.NextLowID = AttackID::Low2;
		attack.NextHighID = AttackID::High1;
		attack.MoutionID = AnimeID::AttackDodgeRoll;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;

		attack.DamageScale = 1.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_DL;

		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::SpeedJumpAttack;
		attack.NextLowID = AttackID::IdleFloatLow;
		attack.NextHighID = AttackID::IdleFloatHigh;
		attack.MoutionID = AnimeID::AttackSpeedJump;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;
		attack.UseGravity = false;

		attack.DamageScale = 1.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_SJL;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackRHigh;
		attack.DamageType = DamageType::HighDamage;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 9.0f / 30.0f;
		attack.OnDamageEnd = 19.0f / 30.0f;
		attack.DamageScale = 1.5f;
		attack.AttackMove = 0.0f;
		attack.FloatMove = 0.0f;
		attack.Rotate = true;
		attack.UseGravity = true;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_H1;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1;
		attack.NextLowID = AttackID::FloatLow2;
		attack.NextHighID = AttackID::FloatLow1FloatHigh;
		attack.MoutionID = AnimeID::AttackFloatLow1;
		attack.EndID = AttackID::FloatLow1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.UseGravity = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 11.0f / 30.0f;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FL1;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow2;
		attack.NextLowID = -1;
		attack.NextHighID = AttackID::FloatLow2FloatHigh;
		attack.MoutionID = AnimeID::AttackFloatLow2;
		attack.EndID = -1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 13.0f / 30.0f;

		attack.DamageScale = 1.5f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FL2;

		attacklist[attack.ID] = attack;

	
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHigh;
		attack.EndID = AttackID::FloatHigh1Fall;
		attack.AttackTime = getMoutionTime(attack.MoutionID);


		//attack.KnockbackEffect = BATTLEACTION::BEATDOWNACTION;
		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;


		attack.UseGravity = false;


		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1Fall;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHighFall;
		attack.EndID = AttackID::FloatHigh1Fall;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.SEID = -1;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;

		attack.UseGravity = true;


		//attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1End;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatHighEnd;
		attack.EndID = -1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 0.0f / 30.0f;


		attack.UseGravity = true;


		//attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FH1;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::HighDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::Special;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackRSP;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.DamageScale = 5.0f;
		attack.AttackMove = 0.0f;
		attack.AddSpecial = 0.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;

		attack.DamageType = DamageType::DethBrowDamage;
		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;
		attack.UseGravity = true;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_PlaySP;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++

		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatLow;
		attack.MoutionID = AnimeID::IdleToFloatLow;
		attack.EndID = AttackID::FloatLow1;
		attack.DamageScale = 0.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.DamageType = DamageType::LowDamage;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;
		attack.SEID = -1;
		attack.Rotate = true;
		attack.UseGravity = false;

		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatHigh;
		attack.MoutionID = AnimeID::IdleToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1FloatHigh;
		attack.MoutionID = AnimeID::FloatLow1ToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow2FloatHigh;
		attack.MoutionID = AnimeID::FloatLow2ToFloatHigh;
		attack.EndID = AttackID::FloatHigh1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;


		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1End;
		attack.MoutionID = AnimeID::AttackFloatLow1End;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.EndID = -1;
		attack.AddSpecial = 0.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 0.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 0.0f / 30.0f;
		attack.FreeDogde = true;

		attack.DamageScale = 0.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.AttackMove = 0.0f;
		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;

	}

	auto func = [&](WeaponType e) {

		AttackState attack;
		auto& attacklist = m_AttackStateList[e];

		attack.ID = AttackID::Low1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackLow1;
		attack.EndID = AttackID::Low1End;
		attack.AddSpecial = 2.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.DamageScale = 1.0f;
		attack.AttackTime = 9.0f / 30.0f;//getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9.0f / 30.0f;
		attack.AttackMove = 0.0f;
		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_L1;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::DogdeAttack;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.EndID = -1;
		attack.MoutionID = AnimeID::AttackDodgeRoll;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_DL;

		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::SpeedJumpAttack;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackSpeedJump;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.Rotate = false;
		attack.UseGravity = false;

		attack.DamageScale = 1.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_SJL;

		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.EndID = AttackID::High1End;
		attack.MoutionID = AnimeID::AttackHigh1;
		attack.DamageType = DamageType::HighDamage;
		attack.AttackTime = 16.0f / 30.0f;
		attack.Rotate = true;
		attack.UseGravity = false;

		attack.OnDamageStart = 6.0f / 30.0f;
		attack.OnDamageEnd = 16.0f / 30.0f;

		attack.DamageScale = 2.0f;
		attack.AttackMove = 0.0f;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_H1;

		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackFloatLow1;
		attack.EndID = AttackID::FloatLow1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 11.0f / 30.0f;

		attack.UseGravity = false;

		attack.SEID = (int)SoundManager::SoundSE_ID::Player_SW_FL1;

		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.DamageType = DamageType::LowDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatHigh1;
		attack.SEID = -1;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::Special;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.EndID = -1;
		attack.MoutionID = AnimeID::AttackLow1;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.DamageScale = 1.0f;
		attack.AttackMove = 0.0f;
		attack.AddSpecial = 0.0f;
		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.DamageType = DamageType::DethBrowDamage;
		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 1.0f;
		attack.Rotate = false;
		attacklist[attack.ID] = attack;

		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatLow;
		attack.MoutionID = AnimeID::IdleToFloatLow;
		attack.EndID = AttackID::FloatLow1;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.DamageType = DamageType::LowDamage;
		attack.AttackTime = 0.0f;
		attacklist[attack.ID] = attack;
		attack.Rotate = true;
		//+++++++++++++++++++++++
		attack.ID = AttackID::IdleFloatHigh;
		attack.MoutionID = AnimeID::IdleToFloatLow;
		attack.EndID = AttackID::FloatHigh1;
		attack.OnDamageEnd = 9999.0f / 30.0f;
		attack.DamageType = DamageType::HighDamage;
		attack.AttackTime = 0.0f;
		attacklist[attack.ID] = attack;
		attack.Rotate = true;

		//+++++++++++++++++++++++

		attack.ID = AttackID::Low1End;
		attack.NextLowID = -1;
		attack.NextHighID = -1;
		attack.MoutionID = AnimeID::AttackLow1End;
		attack.EndID = -1;
		attack.AddSpecial = 0.0f;

		attack.KnockbackEffect = BATTLEACTION::WINCEACTION;
		attack.KnockbackEffectPower = 0.0f;

		attack.OnDamageStart = 0.0f / 30.0f;
		attack.OnDamageEnd = 0.0f / 30.0f;
		attack.FreeDogde = true;
		attack.UseGravity = true;

		attack.DamageScale = 0.0f;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.AttackMove = 0.0f;
		attack.AttackFunc = [&]() {};
		attack.DamageType = DamageType::LowDamage;
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::High1End;
		attack.MoutionID = AnimeID::AttackHigh1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attacklist[attack.ID] = attack;
		//+++++++++++++++++++++++
		attack.ID = AttackID::FloatLow1End;
		attack.MoutionID = AnimeID::AttackFloatLow1End;
		attack.AttackTime = getMoutionTime(attack.MoutionID);
		attack.UseGravity = false;
		attacklist[attack.ID] = attack;
	};
	func(WeaponType::Axe);
	func(WeaponType::Other);
	func(WeaponType::SwordDH);
}

//毎フレーム呼ばれます
void PlayerController::Update(){

	if (m_AttackStateList.size() == 0) {
		AttackInitialize();
	}

	if (!m_CharacterControllerComponent) {
		m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
		if (!m_CharacterControllerComponent)return;
	}

	//if (m_BoneHips && m_AnimeModel) {
	//	float model_z = m_AnimeModel->mTransform->Position().z;
	//	float bone_z = m_BoneHips->mTransform->Position().z;
	//	m_AnimeModel->mTransform->Position(XMVectorSet(0, 0, model_z - bone_z, 1));
	//}
	if(m_PlayerStateLock){
		return;
	}

	if (m_PlayerState != PlayerState::Attack) {
		m_AttackCoolDownTimer -= Hx::DeltaTime()->GetDeltaTime();
		m_AttackCoolDownTimer = max(m_AttackCoolDownTimer, 0.0f);
	}


	for (auto& func : m_DelayUpdate) {
		func();
	}
	m_DelayUpdate.clear();

	ComboAdvantage();

	m_StateFunc[m_PlayerState].Excute();

	stateFlip();

	animeFlip();


	m_UpdateCoroutine.remove_if(
		[](auto f) {return f(); }
	);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void PlayerController::Finish(){

}

//コライダーとのヒット時に呼ばれます
void PlayerController::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void PlayerController::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void PlayerController::OnCollideExit(GameObject target){
	(void)target;
}

void PlayerController::SetPlayerState(PlayerState::Enum state)
{
	if (m_PlayerState_Stack == PlayerState::Movie) {
		if (state != PlayerState::Free) {
			return;
		}
	}
	if (state < 0)return;
	if (state >= PlayerState::Count)return;

	m_PlayerState_Stack = state;
}

PlayerController::PlayerState::Enum PlayerController::GetPlayerState()
{
	return m_PlayerState;
}

void PlayerController::PlayKnockBack(const XMVECTOR& attackVect, KnockBack::Enum knockBackLevel)
{
	switch (knockBackLevel)
	{
	case PlayerController::KnockBack::None:
		break;
	case PlayerController::KnockBack::None_NoInvisible:
		break;
	case PlayerController::KnockBack::Low:
		//m_InvisibleTime = 0.5f;
		mVelocity = attackVect;
		SetPlayerState(PlayerState::KnockBack);

		SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_CV_Damage1, gameObject->mTransform->WorldPosition());
		break;
	case PlayerController::KnockBack::Down:
		//m_InvisibleTime = 2.0f;
		mVelocity = attackVect;
		SetPlayerState(PlayerState::Down);

		SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_CV_Damage2, gameObject->mTransform->WorldPosition());
		break;
	default:
		break;
	}
}

void PlayerController::Damage(float damage, const XMVECTOR& attackVect, KnockBack::Enum knockBackLevel,bool DodgeInevitable, bool GuardInevitable)
{
	if (m_CurrentAttack.ID == AttackID::Special) {
		return;
	}
	if (!GuardInevitable && IsGuard() && XMVector3Dot(gameObject->mTransform->Forward(), attackVect).x < 0) {
		m_GuardParam.AttackGuard = true;

		if(auto particle = Hx::Instance(m_GuardParticle))
			if(m_WeaponHand)
				if(auto scr = m_WeaponHand->GetScript<WeaponHand>())
					if(auto w = scr->GetHandWeapon())
						particle->mTransform->WorldPosition(w->mTransform->WorldPosition());
		return;
	} 
	//回避不可能技か
	if (!(DodgeInevitable && this->GetPlayerState() == PlayerState::Dodge)) {
		if (IsInvisible()) {
			return;
		}
	}

	PlayKnockBack(attackVect,knockBackLevel);
	AddHP(-damage);
	ClearCombo();
	
	if(auto particle = Hx::Instance(m_BloodParticle)){
		auto addpos = XMVectorSet(0,1,0,1);
		particle->mTransform->SetParent(gameObject);
		particle->mTransform->Position(addpos);
	}

	SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Damage, gameObject->mTransform->WorldPosition());

}

bool PlayerController::IsInvisible()
{
	return m_IsInvisible;
}

bool PlayerController::IsDead()
{
	return m_IsDead;
}

bool PlayerController::IsDogde()
{
	return m_PlayerState == PlayerState::Dodge;
}

bool PlayerController::IsGuard()
{
	return m_IsGuard;
}

void PlayerController::SetSpecial(float power)
{
	m_SpecialPower = power;
	m_SpecialPower = min(max(m_SpecialPower, 0.0f), m_SpecialPowerMax);
}

void PlayerController::AddSpecial(float power)
{
	bool playSE = false;
	if (m_SpecialPower < m_SpecialPowerMax) {
		playSE = true;
	}
	m_SpecialPower += power;
	m_SpecialPower = min(max(m_SpecialPower,0.0f), m_SpecialPowerMax);
	if (playSE) {
		if (m_SpecialPower == m_SpecialPowerMax) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SP, gameObject->mTransform->WorldPosition());
		}
	}


}

float PlayerController::GetSpecial()
{
	return m_SpecialPower;
}

void PlayerController::SetHP(float power)
{
	m_HP = power;
	m_HP = min(max(m_HP, 0.0f), m_MaxHP);
	if (m_HP == 0.0f) {
		Dead();
	}
}

void PlayerController::AddHP(float power)
{
	m_HP += power;
	m_HP = min(max(m_HP, 0.0f), m_MaxHP);
	if (m_HP == 0.0f) {
		Dead();
	}
}

float PlayerController::GetHP()
{
	return m_HP;
}

void PlayerController::SetMaxHP(float power)
{
	m_MaxHP = power;
	SetHP(m_HP);
}

void PlayerController::AddMaxHP(float power)
{
	m_MaxHP += power;
	SetHP(m_HP);
}

float PlayerController::GetMaxHP()
{
	return m_MaxHP;
}

void PlayerController::Dead()
{
	m_HP = 0.0f;
	m_IsDead = true;
	m_IsInvisible = true;

	SetPlayerState(PlayerState::Dead);
}

void PlayerController::AddCoroutine(const std::function<bool(void)>& func)
{
	m_UpdateCoroutine.push_back(func);
}

void PlayerController::AddDelayUpdate(const std::function<void(void)>& func)
{
	m_DelayUpdate.push_back(func);
}

int PlayerController::GetHitComboCount()
{
	return m_HitCount;
}

void PlayerController::SpeedJump(const XMVECTOR & vect)
{
	m_MoveVelo = vect;
	SetPlayerState(PlayerState::SpeedJump);
}

void PlayerController::SpeedJumpWeaponCatch(GameObject weapon,bool attack)
{
	PlayerState::Enum mode = PlayerState::Free;
	if (attack) {
		m_NextAttack = AttackID::SpeedJumpAttack;
		mode = PlayerState::Attack;
	}
	else {
		mJump.y += m_JumpPower * 2;
		mode = PlayerState::Free;
	}
	if (weapon) {
		//if (XMVector3Length(m_MoveVelo).x != 0.0f) {
			if (auto w = weapon->GetScript<Weapon>()) {
				if (auto tar = w->GetMirrorTarget()) {
					auto pos = tar->mTransform->WorldPosition();
					auto ppos = gameObject->mTransform->WorldPosition();
					ppos -= m_MoveVelo;
					ppos.y = pos.y;
					auto v = XMVector3Normalize(ppos - pos) * 3.0f;
					pos += v;
					v = pos - gameObject->mTransform->WorldPosition();
					m_CharacterControllerComponent->Move(v);

					pos = tar->mTransform->WorldPosition();
					ppos = gameObject->mTransform->WorldPosition();
					mVelocity = XMVector3Normalize(pos - ppos);
					auto r = m_RotateLimit;
					m_RotateLimit = Init::RotateLimit_dodge;
					rotate();
					m_RotateLimit = r;
				}
			}
			//auto pos = weapon->mTransform->WorldPosition();
			//pos.y -= 0.5f;
			//pos -= XMVector3Normalize(m_MoveVelo)*3.0f;
			//auto ppos = gameObject->mTransform->WorldPosition();
			//m_MoveVelo = pos - ppos;
			//m_CharacterControllerComponent->Move(m_MoveVelo);
		//}
	}
	setWeapon(weapon,true);

	SetPlayerState(mode);
	//SetPlayerState(PlayerState::Free);
}

void PlayerController::AddMove(XMVECTOR v)
{
	if(m_CharacterControllerComponent)
	m_CharacterControllerComponent->Move(v);
}

void PlayerController::SetPlayerLock(bool lock)
{
	m_PlayerStateLock = lock;
}


Weapon * PlayerController::GetWeapon()
{
	auto weaponhand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponhand) {
		if (auto weapon = weaponhand->GetHandWeapon()) {
			if (auto w = weapon->GetScript<Weapon>()) {
				return w;
			}
		}
	}
	return nullptr;
}

GameObject PlayerController::GetLockonTarget()
{
	if (m_Camera) {
		if (auto cam = m_Camera->GetScript<TPSCamera>()) {
			return cam->GetLookTarget();
		}
	}
	return NULL;
}

void PlayerController::LockEnter()
{
}

void PlayerController::LockExcute()
{
	changeAnime(AnimeID::Idle);
	SetPlayerState(PlayerState::Free);
}

void PlayerController::LockExit()
{
}

void PlayerController::FreeEnter()
{
	m_MoveX = 0.0f;
	m_MoveZ = 0.0f;
}

void PlayerController::FreeExcute()
{


	if (m_CurrentAnimeID == AnimeID::Throw) {
		if (m_Camera) {
			mVelocity = m_Camera->mTransform->Forward();
		}
		if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
			if (anime->IsAnimationEnd(AnimeID::Throw)) {
				changeAnime(AnimeID::Idle);
			}
		}
	}
	else {
		move();
		freeAnimeUpdate();
	}

	moveUpdate();

	lockOn();

	rotate();

	//周りの武器の取得,選択関連
	GettingWeapon();

	//if (m_InputF_Time > 0.5f) {
	//	freeAnimeUpdate();
	//	return;
	//}

	throwWeapon();

	if (dodge()) {
		SetPlayerState(PlayerState::Dodge);
		return;
	}


	if (!m_WeaponHand)return;

	//if (BindInput(PlayerInput::ReleaseWeapon)) {
	//	throwAway();
	//}

	if (BindInput(PlayerInput::Guard)) {
		guard();
	}
	if (IsGuard()) {
		SetPlayerState(PlayerState::Guard);
	}

	//{
	//	freeAnimeUpdate();
	//}
	if (attack()) {
		SetPlayerState(PlayerState::Attack);
	}
}

void PlayerController::FreeExit()
{
	if (mAimController && m_Camera) {

		if (auto camera = m_Camera->GetScript<TPSCamera>()) {
			if (auto aim = mAimController->GetScript<AimController>()) {
				m_FreeAIMMode = false;
				aim->ChangeAimMode(camera, gameObject, false);
			}
		}
	}
	m_FreeAIMMode = false;


	//カウントをリセット
	m_InputF_Time = 0.0f;

	auto timeMgr = m_TimeManager->GetScript<TimeManager>();
	if (!timeMgr) return;
	timeMgr->OffSlow();
}

void PlayerController::GuardEnter()
{
	m_GuardParam.AttackGuard = false;
	if (m_GuardParam.JustTimeCooldomn == 0.0f) {
		m_GuardParam.JustTime = 0.1f;

		AddCoroutine([&]()->bool{
			float time = Hx::DeltaTime()->GetDeltaTime();
			m_GuardParam.JustTimeCooldomn -= time;
			m_GuardParam.JustTimeCooldomn = max(m_GuardParam.JustTimeCooldomn, 0.0f);
			return m_GuardParam.JustTimeCooldomn == 0.0f;
		});
	}

	m_GuardParam.JustTimeCooldomn = 0.2f;

	m_IsGuard = true;
	m_RotateLimit = Init::RotateLimit_guard;

	mJump.x = 0.0f;
	mJump.z = 0.0f;

	m_MoveVelo = XMVectorZero();


	SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_CV_Gurad, gameObject->mTransform->WorldPosition());
}

void PlayerController::GuardExcute()
{
	float time = Hx::DeltaTime()->GetDeltaTime();


	//if (BindInput(PlayerInput::ReleaseWeapon)) {
	//	throwAway();
	//	SetPlayerState(PlayerState::Free);
	//	return;
	//}

	guard();

	if (m_GuardParam.AttackGuard) {

		SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Gurad, gameObject->mTransform->WorldPosition());

		if (m_GuardParam.JustTime > 0.0f) {
			m_GuardParam.JustTimeCooldomn = 0.0f;
		}
		else {
			m_GuardParam.KnockBackTime = 0.5f;
		}
		if (m_WeaponHand) {
			if (auto wh = m_WeaponHand->GetScript<WeaponHand>()) {
				if (auto weapon = wh->GetHandWeapon()) {
					if (auto w = weapon->GetScript<Weapon>()) {
						w->Damage(DamageType::HighDamage, m_WeaponResist_ComboAdd);
					}
				}
			}
		}
		m_GuardParam.AttackGuard = false;
	}

	m_GuardParam.JustTime -= time;
	m_GuardParam.JustTime = max(m_GuardParam.JustTime, 0.0f);

	if (m_GuardParam.KnockBackTime > 0.0f) {
		m_GuardParam.KnockBackTime -= time;

		m_MoveVelo = -gameObject->mTransform->Forward();
		m_MoveVelo.y = 0.0f;
		m_MoveVelo = XMVector3Normalize(m_MoveVelo) * 2;
		moveUpdate();
		changeAnime(AnimeID::Guard_Knockback);
	}else{

		m_MoveVelo = XMVectorZero();


		dontmove();
		moveUpdate();
		lockOn();
		rotate();

		if (!BindInput(PlayerInput::Guard)) {
			SetPlayerState(PlayerState::Free);
		}

		if (dodge()) {
			SetPlayerState(PlayerState::Dodge);
		}

		changeAnime(AnimeID::Guard);
	}

}

void PlayerController::GuardExit()
{
	m_IsGuard = false;
	m_GuardParam.JustTime = 0.0f;
	m_RotateLimit = Init::RotateLimit_default;
}

void PlayerController::AttackEnter()
{
	if (m_CurrentAttack.ID == (int)AttackID::Special) {
		m_MoutionSpeed = 1.0f;
	}
	else {
		m_MoutionSpeed = m_MoutionSpeed_ComboAdd;
	}

	currentAttackChange(m_NextAttack);

	mJump.x = 0.0f;
	mJump.z = 0.0f;


	m_RotateLimit = Init::RotateLimit_attack;

	m_AttackCoolDownTimer = 0.25f;

}

void PlayerController::AttackExcute()
{
	if (m_CurrentAttack.ID == (int)AttackID::Special) {
		m_MoutionSpeed = 1.0f;
		SetPlayerState(PlayerState::Special);
		SetSpecial(0.0f);
	}
	else {
		m_MoutionSpeed = m_MoutionSpeed_ComboAdd;
	}
	dontmove();

	moveUpdate();

	lockOn();
	if (m_CurrentAttack.Rotate) {
		rotate();
	}

	
	//if (m_NextAttack==-1) {
	{
		if (BindInput(PlayerInput::ATK_S)) {
			if (GetSpecial() >= m_SpecialPowerMax) {
				m_NextAttack = AttackID::Special;
			}
		}
		else if (BindInput(PlayerInput::ATK_L)) {
			m_NextAttack = m_CurrentAttack.NextLowID;
			m_JumpLowAttack = true;
		}
		else if (BindInput(PlayerInput::ATK_H)) {
			m_NextAttack = m_CurrentAttack.NextHighID;
			m_JumpHighAttack = true;
		}

		if (dodge()) {
			m_NextAttack = 9999;
		}
	}

	float time = Hx::DeltaTime()->GetDeltaTime() * m_MoutionSpeed;
	m_CurrentAttack.AttackTime -= time;
	m_CurrentAttack.OnDamageStart -= time;
	m_CurrentAttack.OnDamageEnd -= time;


	if (m_CurrentAttack.OnDamageEnd <= 0.0f) {
		if (auto w = GetWeapon()) {
			w->SetAttackFlag(false);
		}

		if (m_NextAttack == 9999) {

			SetPlayerState(PlayerState::Dodge);
			return;
		}
	}else if (m_CurrentAttack.OnDamageStart <= 0.0f) {
		if (auto w = GetWeapon()) {
			w->SetAttackFlag(true);
		}
	}

	if (m_CurrentAttack.FreeDogde) {
		if (dodge()) {
			SetPlayerState(PlayerState::Dodge);
			return;
		}
	}

	if (m_CurrentAttack.ID == AttackID::FloatHigh1Fall) {
		if (m_IsGround) {
			m_CurrentAttack.AttackTime = 0.0f;
			m_NextAttack = AttackID::FloatHigh1End;
		}
		else {
			mJump.y = 0.0f;
		}
	}

	if (m_CurrentAttack.AttackTime > 0.0f) {
		m_MoveVelo = gameObject->mTransform->Forward() * m_CurrentAttack.AttackMove;

		if (m_Camera) {
			auto camera = m_Camera->GetScript<TPSCamera>();
			if (m_LockOnEnabled && camera && camera->GetLookTarget()) {
				auto v = camera->GetLookTarget()->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
				m_MoveVelo = XMVector3Normalize(v) * m_CurrentAttack.AttackMove;
			}
		}

		return;
	}



	m_MoveVelo = XMVectorZero();

	if (m_NextAttack == 9999) {

		SetPlayerState(PlayerState::Dodge);
		return;
	}

	if (m_NextAttack >= 0) {
		auto w = GetWeapon();
		if (w) {
			if (m_CurrentAttack.DamageType == DamageType::DethBrowDamage) {
				w->Damage(m_CurrentAttack.DamageType, m_WeaponResist_ComboAdd);
			}
		}

		currentAttackChange(m_NextAttack);

		if (auto w = GetWeapon()) {
			w->SetAttackFlag(false);
		}
		return;
	}

	m_NextAttack = m_CurrentAttack.EndID;

	if (m_NextAttack >= 0) {
		auto w = GetWeapon();
		if (w) {
			if (m_CurrentAttack.DamageType == DamageType::DethBrowDamage) {
				w->Damage(m_CurrentAttack.DamageType, m_WeaponResist_ComboAdd);
			}
		}

		currentAttackChange(m_NextAttack);

		if (auto w = GetWeapon()) {
			w->SetAttackFlag(false);
		}

		return;
	}

	auto w = GetWeapon();
	if (w) {
		if (m_CurrentAttack.DamageType == DamageType::DethBrowDamage) {
			w->Damage(m_CurrentAttack.DamageType, m_WeaponResist_ComboAdd);
		}
	}
	SetPlayerState(PlayerState::Free);
}

void PlayerController::AttackExit()
{
	if (auto w = GetWeapon()) {
		w->SetAttackFlag(false);
	}
	m_MoutionSpeed = 1.0f;

	m_CurrentAttack = AttackState();

	m_NextAttack = -1;

	m_RotateLimit = Init::RotateLimit_default;

	m_MoveVelo = XMVectorZero();

	m_UseGravity = true;
}

void PlayerController::SpecialEnter()
{
	//Hx::DeltaTime()->SetTimeScale(0.0f);
	m_MoutionSpeed = 1.0f;

	currentAttackChange((int)AttackID::Special);

	mJump.x = 0.0f;
	mJump.z = 0.0f;

	if (auto w = GetWeapon()) {
		w->SetAttackFlag(true);
	}

	if (auto gimick = OutputGimic::GetOutputGimic(m_SpecialGimick)) {
		gimick->OnStart(gameObject);
	}
}

void PlayerController::SpecialExcute()
{

	if (!m_AnimeModel)return;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return;

	m_CurrentAnime_Weight = 1.0f;

	auto endtime = anime->GetTotalTime(m_CurrentAnimeID);
	auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
	float timescale = Hx::DeltaTime()->GetTimeScale();
	float scale = 1.0f - timescale;
	p.mTime += Hx::DeltaTime()->GetNoScaleDeltaTime() * 30.0f * scale;
	p.mTime = min(p.mTime, endtime);
	p.mWeight = 1.0f;
	p.mTimeScale = 1.0f;
	anime->SetAnimetionParam(m_CurrentAnimeID, p);

	if (p.mTime == endtime) {
		SetPlayerState(PlayerState::Free);
	}

}

void PlayerController::SpecialExit()
{
	//Hx::DeltaTime()->SetTimeScale(1.0f);
	if (auto w = GetWeapon()) {
		w->Damage(DamageType::DethBrowDamage, m_WeaponResist_ComboAdd);
		w->SetAttackFlag(false);
	}
	m_MoutionSpeed = 1.0f;

	m_CurrentAttack = AttackState();

	m_NextAttack = -1;

	m_MoveVelo = XMVectorZero();

	m_UseGravity = true;
}

void PlayerController::DodgeEnter()
{
	m_MoutionSpeed = m_MoutionSpeed_ComboAdd;
	m_RotateLimit = Init::RotateLimit_dodge;

	//if (m_BoneMirrorObject) {
	//	m_BoneMirrorObject->Disable();
	//}


	float x = 0, y = 0;
	if (BindInput(PlayerInput::Move_F)) {
		y = 1.0f;
	}
	if (BindInput(PlayerInput::Move_D)) {
		y = -1.0f;
	}
	if (BindInput(PlayerInput::Move_L)) {
		x = -1.0f;
	}
	if (BindInput(PlayerInput::Move_R)) {
		x = 1.0f;
	}
	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	if (XMVector2Length(ls).x > 0.05f) {
		x = ls.x;
		y = ls.y;
	}

	auto v = XMVectorSet(x,0.0f,y,1.0f);
	if (abs(v.x) == 0 && abs(v.z) == 0) {
		v = gameObject->mTransform->Forward();
	}
	else {
		if (m_Camera) {
			auto f = m_Camera->mTransform->Forward();
			f.y = 0.0f;
			f = XMVector3Normalize(f);

			auto l = m_Camera->mTransform->Left();
			l.y = 0.0f;
			l = XMVector3Normalize(l);

			auto v2 = XMVectorZero();
			v2 += v.z * f;
			v2 += v.x * l;

			v2.y = 0.0f;
			if (XMVector3Length(v2).x != 0)
				v = XMVector3Normalize(v2);
		}
	}




	mVelocity = v;
	mVelocity.y = 0.0f;
	mJump = XMVectorZero();

	
	m_DogdeParam.HipsZ = 0.0f;
	m_DogdeParam.Timer = (24.0f*2.0f)/60.0f;

	m_MoveVelo = XMVector3Normalize(v) * GetMovementSpeed() * 1.1f;
	m_MoveVelo = XMVectorZero();
	mJump.y += m_JumpPower / 2.0f;
	
	m_IsInvisible=true;

	WeaponType t = WeaponType::Count;
	if (auto w = GetWeapon()) {
		t = w->GetWeaponType();
	}
	if (t == WeaponType::Rance) {
		changeAnime(AnimeID::RDodge);
	}
	else {
		changeAnime(AnimeID::Dogde);
	}

	rotate();


	SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Duchroll, gameObject->mTransform->WorldPosition());
}

void PlayerController::DodgeExcute()
{
	float time = Hx::DeltaTime()->GetDeltaTime();
	//if (m_BoneHips) {
	//	auto model = m_AnimeModel->mTransform->Position();
	//	//auto bone = m_BoneHips->mTransform->Position();
	//	//model.y = 0.0f;
	//	//bone.y = 0.0f;
	//	//bone -= model;
	//	float bone_z = model.z;
	//	if (time != 0) {
	//		m_MoveVelo = m_AnimeModel->mTransform->Forward() * (bone_z - m_DogdeParam.HipsZ) * (1.0f/time);
	//	}
	//	else {
	//		m_MoveVelo = XMVectorZero();
	//	}
	//	m_DogdeParam.HipsZ = bone_z;
	//
	//}

	//AddDelayUpdate([&]() {
	//	if (m_BoneHips) {

	//		//float time = Hx::DeltaTime()->GetDeltaTime();
	//		//float model_z = m_AnimeModel->mTransform->Position().z;
	//		float bone_z  = m_BoneHips->mTransform->Position().z;
	//		//m_AnimeModel->mTransform->Position(XMVectorSet(0, 0, model_z - bone_z, 1));
	//		if (time != 0) {
	//			//float _z = bone_z - m_DogdeParam.HipsZ;
	//			m_MoveVelo = m_AnimeModel->mTransform->Forward() * -bone_z / time;
	//		}
	//		else {
	//			m_MoveVelo = XMVectorZero();
	//		}
	//		//m_DogdeParam.HipsZ = bone_z;
	//	}
	//});


	m_MoutionSpeed = m_MoutionSpeed_ComboAdd;

	moveUpdate();
	lockOn();

	m_DogdeParam.Timer -= time* m_MoutionSpeed;

	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponHand && weaponHand->ActionFree()) {
		if (m_IsGround) {
			if (BindInput(PlayerInput::ATK_L)) {
				m_NextAttack = AttackID::DogdeAttack;
			}
		}
	}
	WeaponType t = WeaponType::Count;
	if (auto w = GetWeapon()) {
		t = w->GetWeaponType();
	}
	if (t == WeaponType::Rance) {
		changeAnime(AnimeID::RDodge);
	}
	else {
		changeAnime(AnimeID::Dogde);
	}

	if (m_DogdeParam.Timer <= 0.0f) {
		if (m_NextAttack != -1) {
			SetPlayerState(PlayerState::Attack);
			return;
		}
	}

	if (m_AnimeModel) {
		auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
		if (anime->IsAnimationEnd(AnimeID::Dogde)|| anime->IsAnimationEnd(AnimeID::RDodge)) {
			SetPlayerState(PlayerState::Free);
		}
	}
	else {
		SetPlayerState(PlayerState::Free);

	}


	//changeAnime(AnimeID::Dogde);
}

void PlayerController::DodgeExit()
{
	//if (m_BoneMirrorObject) {
	//	m_BoneMirrorObject->Enable();
	//}
	m_MoutionSpeed = 1.0f;

	m_DogdeParam.Timer = 0.0f;
	m_IsInvisible=false;
	m_MoveVelo = XMVectorZero();
	m_RotateLimit = Init::RotateLimit_default;

	//AddDelayUpdate([&]() {
	//	m_MoveVelo = XMVectorZero();
	//});
}

void PlayerController::SpeedJumpEnter()
{
	//m_IsInvisible = true;
	m_UseGravity = false;
	mJump = XMVectorZero();

	mVelocity = m_MoveVelo;
}

void PlayerController::SpeedJumpExcute()
{
	moveUpdate();
	lockOn();
	rotate();

	changeAnime(AnimeID::SpeedJump);
}

void PlayerController::SpeedJumpExit()
{
	//m_IsInvisible = false;
	m_UseGravity = true;
	m_MoveVelo = XMVectorZero();
}

void PlayerController::KnockBackEnter()
{
	m_IsInvisible = true;
	//mJump.y += mVelocity.y;
	//m_MoveVelo.y = 0.0f;
	//m_MoveVelo.x = mVelocity.x;
	//m_MoveVelo.z = mVelocity.z;
	mVelocity *= -1;
	mVelocity.y = 0.0f;
	
	mJump = XMVectorZero();
	m_MoveVelo = XMVectorZero();
	rotate();

	m_InvisibleTime = 0.0f;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (anime) {
		m_InvisibleTime = anime->GetTotalTime(AnimeID::KnockBack) / 60.0f;
	}

}

void PlayerController::KnockBackExcute()
{
	moveUpdate();

	lockOn();

	float time = Hx::DeltaTime()->GetDeltaTime();
	m_InvisibleTime -= time;
	if (m_InvisibleTime <= 0.0f) {
		SetPlayerState(PlayerState::Free);
	}
	changeAnime(AnimeID::KnockBack);
}

void PlayerController::KnockBackExit()
{
	m_MoveVelo = XMVectorZero();
	m_InvisibleTime = 0.0f;
	m_IsInvisible = false;
}


void PlayerController::DownEnter()
{
	m_IsInvisible = true;
	m_DownUp = false;
	//mJump.y += mVelocity.y;
	//m_MoveVelo.y = 0.0f;
	//m_MoveVelo.x = mVelocity.x;
	//m_MoveVelo.z = mVelocity.z;
	mVelocity *= -1;
	mVelocity.y = 0.0f;

	m_InvisibleTime = 0.0f;
	if (m_AnimeModel) {
		auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
		if (anime) {
			m_InvisibleTime = anime->GetTotalTime(AnimeID::Down) / 60.0f + 5.0f;
		}
	}

	mJump = XMVectorZero();
	m_MoveVelo = XMVectorZero();
	rotate();


	SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Down, gameObject->mTransform->WorldPosition());
}

void PlayerController::DownExcute()
{
	moveUpdate();

	lockOn();

	float time = Hx::DeltaTime()->GetDeltaTime();
	m_InvisibleTime -= time;


	if (m_DownUp) {
		changeAnime(AnimeID::DownUp);
		if (m_InvisibleTime <= 0.0f) {
			SetPlayerState(PlayerState::Free);
		}
	}
	else {


		bool move = false;
		if (BindInput(PlayerInput::Move_F) || BindInput(PlayerInput::Move_D) || BindInput(PlayerInput::Move_L) || BindInput(PlayerInput::Move_R)) {
			move = true;
		}
		auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
		if (XMVector2Length(ls).x > 0.05f) {
			move = true;
		}

		if (m_InvisibleTime <= 5.0f && m_IsGround && (move || m_InvisibleTime <= 0.0f)) {
			m_DownUp = true;

			if (m_AnimeModel) {
				if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
					m_InvisibleTime = anime->GetTotalTime(AnimeID::DownUp) / 30.0f;
				}
			}
		}
		changeAnime(AnimeID::Down);
	}
}

void PlayerController::DownExit()
{
	m_MoveVelo = XMVectorZero();
	m_InvisibleTime = 0.0f;
	m_IsInvisible = false;
}

void PlayerController::MovieEnter()
{
}

void PlayerController::MovieExcute()
{
}

void PlayerController::MovieExit()
{
}

void PlayerController::DeadEnter()
{
	m_IsInvisible = true;
	m_IsDead = true;
	mJump = XMVectorZero();
	SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_CV_Dead, gameObject->mTransform->WorldPosition());
}

#include "ChangeScene.h"
void PlayerController::DeadExcute()
{
	lockOn();
	changeAnime(AnimeID::Down);

	m_GameOverTime += Hx::DeltaTime()->GetDeltaTime();
	if (m_GameOverTime >= 3.0f) {
		if (m_ChangeSceneGameOver) {
			if (auto c = m_ChangeSceneGameOver->GetScript<ChangeScene>()) {
				c->Next();
			}

		}
	}

}

void PlayerController::DeadExit()
{
	m_IsInvisible = false;
	m_IsDead = false;
}

void PlayerController::move()
{

	float time = Hx::DeltaTime()->GetDeltaTime();
	float speed = GetMovementSpeed();
	float x = 0, y = 0;
	if (BindInput(PlayerInput::Move_F)) {
		y = 1.0f;
	}
	if (BindInput(PlayerInput::Move_D)) {
		y = -1.0f;
	}
	if (BindInput(PlayerInput::Move_L)) {
		x = -1.0f;
	}
	if (BindInput(PlayerInput::Move_R)) {
		x = 1.0f;
	}
	float max_x = 1.0f;
	float max_z = 1.0f;

	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	if (XMVector2Length(ls).x > 0.05f) {
		x = ls.x;
		y = ls.y;

		max_x = abs(x);
		max_z = abs(y);
	}

	float kansei = 10.0f * time;
	if (x == 0.0f && m_MoveX != 0.0f) {
		if (abs(m_MoveX) <= kansei) {
			x = -m_MoveX;
		}
		else {
			x = -(m_MoveX / abs(m_MoveX));
			x *= kansei;
		}
	}
	else {
		x *= kansei;
	}
	if (y == 0.0f && m_MoveZ != 0.0f) {
		if (abs(m_MoveZ) <= kansei) {
			y = -m_MoveZ;
		}
		else {
			y = -(m_MoveZ / abs(m_MoveZ));
			y *= kansei;
		}
	}
	else {
		y *= kansei;
	}

	m_MoveX += x;
	m_MoveZ += y;

	m_MoveX = min(max(m_MoveX, -max_x), max_x);
	m_MoveZ = min(max(m_MoveZ, -max_z), max_z);

	auto xy = XMVectorSet(m_MoveX, m_MoveZ, 0, 1);
	auto l = XMVector2Length(xy).x;
	if (l>=0.01f) {
		l = min(max(l, -1.0f), 1.0f);
		xy = XMVector2Normalize(xy) * l;
	}
	else {
		xy = XMVectorZero();
	}

	//m_IsGround = m_CharacterControllerComponent->IsGround();

	//XMVECTOR pos = gameObject->mTransform->WorldPosition();
	//if (m_IsGround) {
	//	auto d = XMVectorSet(0, -1, 0, 1);
	//	RaycastHit hit;
	//	if (Hx::PhysX()->RaycastHit(pos, d, 100.0f, &hit,Layer::UserTag4)) {
	//		auto dot = XMVector3Dot(hit.normal, XMVectorSet(0, 1, 0, 1)).x;
	//		auto angle = dot;

	//		auto deg = m_CharacterControllerComponent->GetSlopeLimit();
	//		float slopeLimit = cosf(XM_PI / 180.0f * deg);
	//		if (slopeLimit > angle) {
	//			m_IsGround = false;

	//			hit.normal.y = 0.0f;
	//			hit.normal = XMVector3Normalize(hit.normal);
	//			auto v2 = mJump + hit.normal * speed * 0.2f;
	//			v2.y = 0.0f;
	//			auto s = min(max(XMVector3Length(v2).x, -speed), speed);
	//			v2 = XMVector3Normalize(v2)*s;
	//			mJump.x = v2.x;
	//			mJump.z = v2.z;
	//		}
	//	}
	//}
	auto v = XMVectorZero();

	if (m_Camera) {
		bool end = false;
		auto camera = m_Camera->GetScript<TPSCamera>();
		if (camera) {
			if (auto target = camera->GetLookTarget()) {
				auto vect = target->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
				//vect.y = 0.0f;
				vect = XMVector3Normalize(vect); 
				auto l = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 1), vect));
				auto f = XMVector3Normalize(XMVector3Cross(l, XMVectorSet(0, 1, 0, 1)));
				v += xy.y * f;
				v += xy.x * l;
				end = true;
				v.y = 0.0f;
				mVelocity = XMVector3Normalize(v);
			}
		}
		if (!end) {
			auto f = m_Camera->mTransform->Forward();
			//f.y = 0.0f;
			//auto l = m_Camera->mTransform->Left();
			//l.y = 0.0f;


			auto l = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 1), f));
			f = XMVector3Normalize(XMVector3Cross(l, XMVectorSet(0, 1, 0, 1)));
			
			v += xy.y * XMVector3Normalize(f);
			v += xy.x * XMVector3Normalize(l);
		}

		v.y = 0.0f;
		//if(XMVector3Length(v).x != 0)
		//v = XMVector3Normalize(v);

		if (!m_LockOnEnabled || xy.x != 0.0f || xy.y != 0.0f) {
			mVelocity = v;
		}
	}

	if (!m_IsSlopeLimited) {

		if (m_IsGround) {
			if (BindInput(PlayerInput::Jump)) {
				mJump.y += m_JumpPower;
			}
		}

		v *= speed;

		mJump.x = v.x;
		mJump.z = v.z;
	}

}

void PlayerController::dontmove()
{

	float time = Hx::DeltaTime()->GetDeltaTime();
	float speed = GetMovementSpeed();
	float x = 0, y = 0;
	if (BindInput(PlayerInput::Move_F)) {
		y = 1.0f;
	}
	if (BindInput(PlayerInput::Move_D)) {
		y = -1.0f;
	}
	if (BindInput(PlayerInput::Move_L)) {
		x = -1.0f;
	}
	if (BindInput(PlayerInput::Move_R)) {
		x = 1.0f;
	}


	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	if (XMVector2Length(ls).x > 0.05f) {
		x = ls.x;
		y = ls.y;
	}
	
	auto xy = XMVectorZero();
	if (XMVector2Length(XMVectorSet(x, y, 0, 1)).x != 0.0f) {
		xy = XMVector2Normalize(XMVectorSet(x, y, 0, 1));
	}
	auto v = XMVectorZero();
	if (m_Camera) {
		auto f = m_Camera->mTransform->Forward();
		f.y = 0.0f;
		f = XMVector3Normalize(f);

		auto l = m_Camera->mTransform->Left();
		l.y = 0.0f;
		l = XMVector3Normalize(l);

		v += xy.y * f;
		v += xy.x * l;

		v.y = 0.0f;
		if (XMVector3Length(v).x != 0)
			v = XMVector3Normalize(v);
	}

	if (XMVector3Length(v).x != 0) {
		mVelocity = v;
	}

}
void PlayerController::moveUpdate()
{
	float time = Hx::DeltaTime()->GetDeltaTime();

	if (m_UseGravity) {

		mJump += mGravity * time;
	}


	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	if (m_IsSlopeLimited && m_UseGravity) {
		auto d = XMVectorSet(0, -1, 0, 1);
		RaycastHit hit;
		if (Hx::PhysX()->RaycastHit(pos, d, 10.0f, &hit, Layer::UserTag4)) {

			auto dot = XMVector3Dot(hit.normal, XMVectorSet(0, 1, 0, 1)).x;
			auto angle = dot;

			auto deg = m_CharacterControllerComponent->GetSlopeLimit();
			float slopeLimit = cosf(XM_PI / 180.0f * deg);
			if (slopeLimit > angle) {
				m_IsGround = false;
				m_IsSlopeLimited = true;

				auto y = hit.normal.y;
				hit.normal.y = 0.0f;
				hit.normal = XMVector3Normalize(hit.normal);
				float speed = GetMovementSpeed();
				auto v2 = mJump + hit.normal * speed * 0.2f;
				v2.y = 0.0f;
				auto s = min(max(XMVector3Length(v2).x, -speed), speed);
				v2 = XMVector3Normalize(v2)*s;
				mJump.x = v2.x;
				mJump.z = v2.z;
				mJump.y -= y;
			}
			else {
				m_IsSlopeLimited = false;
			}
		}

		m_CharacterControllerComponent->Move(mJump * time);
	}
	else{
		m_IsSlopeLimited = false;
		auto p = XMVectorZero();
		p += mJump * time;
		p += m_MoveVelo * time;

		m_CharacterControllerComponent->Move(p);


		if (m_CharacterControllerComponent->IsGround() && m_UseGravity) {
			auto d = XMVectorSet(0, -1, 0, 1);
			RaycastHit hit;
			if (Hx::PhysX()->RaycastHit(pos, d, 10.0f, &hit, Layer::UserTag4)) {

				auto dot = XMVector3Dot(hit.normal, XMVectorSet(0, 1, 0, 1)).x;
				auto angle = dot;

				auto deg = m_CharacterControllerComponent->GetSlopeLimit();
				float slopeLimit = cosf(XM_PI / 180.0f * deg);
				if (slopeLimit > angle) {
					m_IsGround = false;
					m_IsSlopeLimited = true;
				}
			}
		}
	}

	if (!m_IsSlopeLimited) {
		if (m_UseGravity) {
			if (m_IsGround && !m_CharacterControllerComponent->IsGround() && mJump.y <= 0.0f) {
				XMVECTOR donw = XMVectorSet(0, -m_CharacterControllerComponent->GetStepOffset() , 0, 1);
				m_CharacterControllerComponent->Move(donw);
				if (!m_CharacterControllerComponent->IsGround()) {
					m_CharacterControllerComponent->Move(-donw);
				}
			}
		}
		bool f = false;
		if (!m_IsGround) {
			f = true;
		}
		m_IsGround = m_CharacterControllerComponent->IsGround();

		if (m_IsGround && f) {
			RaycastHit hit;
			if (Hx::PhysX()->RaycastHit(gameObject->mTransform->WorldPosition(), XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f), 0.25f, &hit, Layer::UserTag3)) {
				if (hit.hit) {
					auto epos = hit.hit->mTransform->WorldPosition();
					auto ppos = gameObject->mTransform->WorldPosition();
					epos.y = 0.0f;
					ppos.y = 0.0f;
					auto v = ppos - epos;
					if (XMVector3Length(v).x != 0.0f) {
						v = XMVector3Normalize(v) * 10.0f;
						v *= Hx::DeltaTime()->GetDeltaTime();
						m_CharacterControllerComponent->Move(v);
					}
					m_IsGround = false;
				}
			}
		}

		if (m_IsGround) {
			mJump.y = 0.0f;

			m_JumpThrowWeapon = false;
			m_JumpLowAttack = false;
			m_JumpHighAttack = false;
		}
	}

	BoneMoveUpdate();

}

void PlayerController::rotate()
{
	auto vect = mVelocity;
	vect.y = 0.0f;
	if (XMVector3Length(vect).x == 0)return;
	vect = XMVector3Normalize(vect);
	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 0,0,1), vect, XMVectorSet(0, 1, 0, 1)));
	auto q = XMQuaternionRotationMatrix(rotate);

	auto f = gameObject->mTransform->Forward();
	f.y = 0.0f;
	f = XMVector3Normalize(f);
	float angle =  acos(min(max(XMVector3Dot(f, vect).x,-1.0f),1.0f));
	float limit = m_RotateLimit / 180.0f * XM_PI * Hx::DeltaTime()->GetDeltaTime();
	float t = 1.0f;
	if (limit < angle) {
		t = limit / angle;
	}
	auto wq = gameObject->mTransform->WorldQuaternion();
	q = XMQuaternionSlerp(wq,q,t);
	gameObject->mTransform->WorldQuaternion(q);
}

bool PlayerController::dodge()
{
	//if (m_IsGround) {
		if (BindInput(PlayerInput::Step)) {
			return true;
		}
	//}

	return false;
}

#include "WeaponHand.h"
void PlayerController::guard()
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponHand && weaponHand->ActionFree()) {
		m_IsGuard = true;
	}
}

bool PlayerController::attack()
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (!weaponHand || !weaponHand->ActionFree()) {
		return false;
	}
	if (m_IsGround) {
		if (BindInput(PlayerInput::ATK_S)) {
			if (GetSpecial() >= m_SpecialPowerMax) {
				m_NextAttack = AttackID::Special;
				SetSpecial(0.0f);
				return true;
			}
		}
	}
	if (m_AttackCoolDownTimer > 0.0f) {
		return false;
	}

	if (m_IsGround) {
		if (BindInput(PlayerInput::ATK_L)) {
			m_NextAttack = AttackID::Low1;
		}
		else if (BindInput(PlayerInput::ATK_H)) {
			m_NextAttack = AttackID::High1;
		}

	}
	else {
		if (BindInput(PlayerInput::ATK_L) && !m_JumpLowAttack) {
			m_NextAttack = AttackID::IdleFloatLow;
			m_JumpLowAttack = true;
		}

		if (BindInput(PlayerInput::ATK_H) && !m_JumpHighAttack) {
			m_NextAttack = AttackID::IdleFloatHigh;
			m_JumpHighAttack = true;
		}
	}
	return (m_NextAttack != -1);
}

void PlayerController::throwAway(GameObject target,bool isMove)
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponHand && weaponHand->ActionFree()) {

		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (target) {

			if (m_LockActions[LockAction::ThrowWeapon])return;
			createWeaponEffect(100.0f, (int)WeaponEffectType::THROWEF);
			weaponHand->ThrowAway(target, isMove);

			SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Throw, gameObject->mTransform->WorldPosition());
			
		}
		else{
			if (m_LockActions[LockAction::DropWeapon])return;
			if (auto weapon = weaponHand->GetHandWeapon()) {
				if (auto w = weapon->GetScript<Weapon>()) {
					if (!w->isBreak()) {
						changeAnime(AnimeID::WeaponDrop);
					}
				}
				weaponHand->ThrowAway();

				SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Drop, gameObject->mTransform->WorldPosition());
			}
		}
	}
}

#include "TargetEnemy.h"
void PlayerController::lockOn()
{
	if (!m_Camera)return;
	auto camera = m_Camera->GetScript<TPSCamera>();

	if (camera) {
		camera->SetLeft(0.0f);

		//ロックオンしている(敵)
		if (m_LockOnEnabled) {
			//ロックオンしている敵が存在する
			if (auto target = camera->GetLookTarget()) {
				auto pos = gameObject->mTransform->WorldPosition();
				auto tarpos = target->mTransform->WorldPosition();

				//対象との距離が遠くなったら
				if (XMVector3Length(pos - tarpos).x > 60.0f) {
					m_LockOnEnabled = false;
					camera->SetLookTarget(NULL);
				}
			}
			else {
				//ロックオンを解除
				m_LockOnEnabled = false;
			}
		}
	}

	//Tキー押したら
	if (BindInput(PlayerInput::LockOn)) {
		//ロックオンされていない
		if (!m_LockOnEnabled) {
			if (!m_GetEnemy)return;
			auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

			if (!getenemy)return;
			auto enemy = getenemy->GetMinEnemy();
			//auto enemy = getenemy->GetPointMinEnemy(gameObject, GetEnemy::MinVect::right);
			if (!enemy)return;

			if (camera) {
				camera->SetLookTarget(enemy);
				m_LockOnEnabled = true;
			}
		}
		else {
			m_LockOnEnabled = false;
		}
	}
	camera->SetLeft(0.0f);

	if (!m_LockOnEnabled)return;
	
	/*↓ロックオンされていた場合の処理↓*/

	static bool  inputflag = true;
	if (BindInput(PlayerInput::LockOn_L) && inputflag) {
		inputflag = false;
		if (!m_GetEnemy)return;
		auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

		if (!getenemy)return;
		if (camera) {

			auto enemy = getenemy->GetPointMinEnemy(camera->GetLookTarget(), GetEnemy::MinVect::left);
			if (!enemy)return;
			if (enemy == camera->GetLookTarget())return;
			camera->SetLookTarget(enemy);
		}

	}
	if (BindInput(PlayerInput::LockOn_R) && inputflag) {

		inputflag = false;
		if (!m_GetEnemy)return;
		auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

		if (!getenemy)return;
		if (camera) {

			auto enemy = getenemy->GetPointMinEnemy(camera->GetLookTarget(), GetEnemy::MinVect::right);
			if (!enemy)return;
			if (enemy == camera->GetLookTarget())return;
			camera->SetLookTarget(enemy);
		}
	}
	if (!BindInput(PlayerInput::LockOn_L)&& !BindInput(PlayerInput::LockOn_R)) {
		inputflag = true;
	}

	//LookTargetのレイヤーが"Layer3"(敵)だったなら
	if (camera->GetLookTarget()->GetLayer() == 3) {
		//敵を保存
		camera->SetSaveEnemy(camera->GetLookTarget());
	}

	//auto f = m_Camera->mTransform->Forward();
	//f.y = 0.0f;
	//f = XMVector3Normalize(f);
	//mVelocity = f;

	auto left = m_Camera->mTransform->Left();
	auto v = mJump;
	v.y = 0.0f;
	v = XMVector3Normalize(v);
	float dot = XMVector3Dot(v,left).x;

	if (abs(dot) > 0.5f) {
		float l = dot / abs(dot);
		if (camera) {
			camera->SetLeft(l * 4.0f);
		}
	}
	else {
		camera->SetLeft(2.0f);
	}

}
#include "Enemy.h"
void PlayerController::GettingWeapon(){

	//早期リターン
	if (!m_WeaponHand) return;
	if (!m_GetWeapon) return;
	if (!m_TimeManager) return;
	if (!m_Camera) return;

	//GetWeaponのスクリプトの取得
	//auto getWeapon = GetWeapon();
	auto getWeapon = m_GetWeapon->GetScript<::GetWeapon>();
	if (!getWeapon) return;
	
	//WeaponHandのスクリプトの取得
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (!weaponHand) return;

	//TimeManagerのスクリプトの取得
	auto timeMgr = m_TimeManager->GetScript<TimeManager>();
	if (!timeMgr) return;

	//TPSCameraのスクリプトの取得
	auto camera = m_Camera->GetScript<TPSCamera>();
	if (!camera) return;
	if (!m_marker) return;

	//武器を持っていたら
	if (weaponHand->GetHandWeapon()) {
		//オフ
		m_marker->Disable();
	}

	if (m_tempWeapon) {
		m_marker->Enable();
		XMVECTOR markerPos = m_tempWeapon->mTransform->WorldPosition() + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_marker->mTransform->WorldPosition(markerPos);
	}
	else {
		m_marker->Disable();
	}

	//Fキーを一定時間長押ししたら
	if (m_InputF_Time > 0.5f) {
		//スローモードにする
		timeMgr->OnSlow();

		static bool  inputflag = true;
		if (BindInput(PlayerInput::LockOn_L) && inputflag) {
			inputflag = false;
			//選択対象から左に一番近いものを取得
			auto t = getWeapon->GetPointMinWeapon(m_tempWeapon, GetWeapon::MinVect::left);
			if(t)
			m_tempWeapon = t;

		}
		else if (BindInput(PlayerInput::LockOn_R) && inputflag) {
			inputflag = false;
			//選択対象から右に一番近いものを取得
			auto t = getWeapon->GetPointMinWeapon(m_tempWeapon, GetWeapon::MinVect::right);
			if (t)
				m_tempWeapon = t;
		}
		if (!BindInput(PlayerInput::LockOn_L) && !BindInput(PlayerInput::LockOn_R)) {
			inputflag = true;
		}
		//武器を見る
		camera->SetLookTarget(m_tempWeapon);
	}
	else {
		//ターゲット情報を保存していなかったら
		if (!camera->GetSaveEnemy()) {
//フリールック
camera->SetLookTarget(NULL);
		}
		else {
			//ロックオンしていた
			if (m_LockOnEnabled) {
				camera->SetLookTarget(camera->GetSaveEnemy());
			}
			else {
				camera->SetLookTarget(NULL);
			}

		}
	}

	//Fキーを押している時間をカウント
	if (BindInput(PlayerInput::GetWeaponDown)) {
		m_InputF_Time += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	}
	//Fキーを話したら
	else if (BindInput(PlayerInput::GetWeapon)) {
		//if (m_soundManager) {
		//	auto sound = m_soundManager->GetScript<SoundManager>();
		//	if (!sound) {
		//		Hx::Debug()->Log("SoundManagerないよ");
		//		return;
		//	};
		//}
		throwAway();
		if (m_tempWeapon) {
			//選択した武器をセット
			setWeapon(m_tempWeapon);
		}
		//カウントをリセット
		m_InputF_Time = 0.0f;
		//スローモード解除
		timeMgr->OffSlow();
	}
	else {
		if (weaponHand->GetHandWeapon()) {
			getWeapon->SetExClusionWeapon(weaponHand->GetHandWeapon());
		}
		else {
			getWeapon->SetExClusionWeapon(NULL);
		}
		//常に一番近い武器を取得
		m_tempWeapon = getWeapon->GetMinWeapon();
	}
}

void PlayerController::throwWeapon()
{
	if (m_LockActions[LockAction::ThrowWeapon])return;

	//10 / 29 更新
	if (mMoveAvility) {
		if (BindInput(PlayerInput::GoingWeapon) && mWeaponControl) {
			m_GoingWeapon = true;
		}

		if (m_GoingWeapon) {
			if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
				if (weaponCtr->IsHit())
				{
					weaponCtr->DeleteHitPoint();
					if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
						script->OnMove();

						SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SpeedJump, gameObject->mTransform->WorldPosition());
					}
				}
			}
		}

		if (m_WeaponHand) {
			if (auto weaponHand = m_WeaponHand->GetScript<WeaponHand>()) {
				if (!weaponHand->GetHandWeapon())return;
			}

		}

		if (m_JumpThrowWeapon) {
			return;
		}

		auto camera = m_Camera->GetScript<TPSCamera>();

		if (camera) {
			if (auto aim = mAimController->GetScript<AimController>()) {
				if (BindInput(PlayerInput::FreeAIM) && !m_MapShiftFlag) {

					if (auto scr = m_WeaponHand->GetScript<WeaponHand>()) {
						if (auto target = scr->GetHandWeapon())
						{
							if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
								script->SetPoint(target, this);
							}
						}
					}
					if (m_GetMapShift) {
						if(auto ms = m_GetMapShift->GetScript<GetMapShift>()){
							if (auto shift = ms->GetMinMapShift()) {

								if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
									weaponCtr->DeleteHitPoint();
								}
								throwAway(shift);
								m_JumpThrowWeapon = true;
								m_GoingWeapon = true;

							}
						}
					}

					m_MapShiftFlag = true;
				}
				else if (BindInput(PlayerInput::ThrowWeapon) && !m_MapShiftFlag) {
					if (!m_Camera)return;
					auto camera = m_Camera->GetScript<TPSCamera>();
					if (camera) {
						GameObject target; /*= camera->GetLookTarget();*/
						if (auto scr = m_WeaponHand->GetScript<WeaponHand>()) {
							target = scr->GetHandWeapon();
						}
						auto camtar = camera->GetLookTarget();
						if (target && camtar) {
							if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
								script->SetPoint(target, this);
							}
							if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
								weaponCtr->DeleteHitPoint();
							}
							throwAway(camtar);
							m_JumpThrowWeapon = true;
							m_GoingWeapon = false;
						}
					}
				}
				else if (!BindInput(PlayerInput::FreeAIM)) {
					m_MapShiftFlag = false;
				}
				//if (BindInput(PlayerInput::FreeAIM) && !m_FreeAIMMode) {
				//	changeAnime(AnimeID::FreeAIM);
				//
				//	if (auto scr = m_WeaponHand->GetScript<WeaponHand>()) {
				//		if (auto target = scr->GetHandWeapon())
				//		{
				//			if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
				//				script->SetPoint(target, this);
				//			}
				//		}
				//	}
				//	aim->ChangeAimMode(camera, gameObject, true);
				//	m_FreeAIMMode = true;
				//}
				//if (BindInput(PlayerInput::ThrowWeapon) && m_FreeAIMMode) {
				//	mJump.x=0.0f;
				//	mJump.z=0.0f;
				//	changeAnime(AnimeID::Throw);
				//	m_FreeAIMMode = false;
				//	aim->ChangeAimMode(camera, gameObject, false);
				//	//Hx::Debug()->Log("UP : " + std::to_string(camera->gameObject->mTransform->Forward().x));
				//	//Hx::Debug()->Log("UP : " + std::to_string(camera->gameObject->mTransform->Forward().y));
				//	//Hx::Debug()->Log("UP : " + std::to_string(camera->gameObject->mTransform->Forward().z));
				//	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
				//	if (weaponHand) {
				//		GameObject target = NULL; /*= camera->GetLookTarget();*/
				//		auto camera = m_Camera->GetScript<TPSCamera>();
				//		if (camera) {
				//			if (auto scr = m_WeaponHand->GetScript<WeaponHand>()) {
				//				target = camera->GetLookTarget();
				//			}
				//		}
				//
				//		if (target) {
				//			throwAway(target);
				//		}
				//		else {
				//			weaponHand->ThrowAway(camera->gameObject->mTransform->Forward());
				//		}
				//	}
				//	if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
				//		weaponCtr->DeleteHitPoint();
				//	}
				//	m_JumpThrowWeapon = true;
				//	m_GoingWeapon = false;
				//	//throwAway(camera->gameObject->mTransform->WorldPosition() * -1, true);
				//}
				//else 
				//if (BindInput(PlayerInput::ThrowWeapon)) {
				//	if (!m_Camera)return;
				//	auto camera = m_Camera->GetScript<TPSCamera>();
				//	if (camera) {
				//		GameObject target; /*= camera->GetLookTarget();*/
				//		if (auto scr = m_WeaponHand->GetScript<WeaponHand>()) {
				//			target = scr->GetHandWeapon();
				//		}
				//		auto camtar = camera->GetLookTarget();
				//		if (target && camtar) {
				//			if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
				//				script->SetPoint(target, this);
				//			}
				//			throwAway(camtar);
				//			if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
				//				weaponCtr->DeleteHitPoint();
				//			}
				//			m_JumpThrowWeapon = true;
				//			m_GoingWeapon = false;
				//		}
				//	}
				//}
				//else if(m_FreeAIMMode){
				//	changeAnime(AnimeID::FreeAIM);
				//}

			}
		}

	}
}

void PlayerController::setWeapon(GameObject weapon,bool FastCatch)
{
	//WeaponHandのスクリプトの取得
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (!weaponHand) return;
	weaponHand->SetWeapon(weapon, [&](auto o, Weapon* w, auto t) {
		if (Enemy* scr = Enemy::GetEnemy(o)) {
			if (w->isThrow()) {

				if (scr->Damage(m_ThrowAttack.DamageScale * w->GetAttackPower() * m_ATK_ComboAdd, m_ThrowAttack.KnockbackEffect, XMVectorSet(0, m_ThrowAttack.KnockbackEffectPower, 0, 1))) {
					if (t == Weapon::HitState::Damage) {
						AddSpecial(m_ThrowAttack.AddSpecial);
						AddCombo();

						SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_R_Low_Hit, gameObject->mTransform->WorldPosition());
					}

					if (m_ThrowAttack.DamageType != DamageType::DethBrowDamage) {
						w->Damage(m_ThrowAttack.DamageType, m_WeaponResist_ComboAdd);
					}
				}
			}else if (w->isAttack()) {

				if (scr->Damage(m_CurrentAttack.DamageScale * w->GetAttackPower() * m_ATK_ComboAdd, m_CurrentAttack.KnockbackEffect, XMVectorSet(0, m_CurrentAttack.KnockbackEffectPower, 0, 1))) {
					if (t == Weapon::HitState::Damage) {
						AddSpecial(m_CurrentAttack.AddSpecial);
						AddCombo();
						if (m_CurrentAttack.DamageType == DamageType::LowDamage) {
							if (m_CurrentWeaponType == (int)WeaponType::Rance) {
								SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_R_Low_Hit, gameObject->mTransform->WorldPosition());
							}
							else {
								SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_SW_Low_Hit, gameObject->mTransform->WorldPosition());
							}
						}
						if (m_CurrentAttack.DamageType == DamageType::HighDamage) {
							SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_SW_High_Hit, gameObject->mTransform->WorldPosition());
						}

					}
					if (m_CurrentAttack.DamageType != DamageType::DethBrowDamage) {
						w->Damage(m_CurrentAttack.DamageType, m_WeaponResist_ComboAdd);
					}
				}
				//WeaponType t = w->GetWeaponType();

			}
		}
	}, FastCatch);
	if (mWeaponControl) {
		if (auto weaponCtr = mWeaponControl->GetScript<WeaponControl>()) {
			weaponCtr->DeleteHitPoint();
		}
	}
}

void PlayerController::currentAttackChange(int attackid)
{
	m_NextAttack = attackid;
	if (m_NextAttack >= 0 && m_NextAttack < (int)AttackID::Count) {
		if (auto w = GetWeapon()) {
			WeaponType t = w->GetWeaponType();
			m_CurrentWeaponType = (int)t;
			m_CurrentAttack = m_AttackStateList[m_CurrentWeaponType][m_NextAttack];
		}
	}
	if (m_CurrentAttack.AttackTime <= -1.0f) {
		m_CurrentAttack.AttackTime = getMoutionTime(m_CurrentAttack.MoutionID);
	}
	if (m_CurrentAttack.SEID >= 0) {
		SoundManager::PlaySE((SoundManager::SoundSE_ID::Enum)m_CurrentAttack.SEID, gameObject->mTransform->WorldPosition());

		if (m_CurrentAttack.DamageType == DamageType::LowDamage) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_SW_Low_Swing, gameObject->mTransform->WorldPosition());
		}else if (m_CurrentAttack.DamageType == DamageType::HighDamage) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_SE_SW_High_Swing, gameObject->mTransform->WorldPosition());
		}
	}

	m_CurrentAttack.AttackFunc();

	if (m_CurrentAttack.ID == (int)AttackID::Special) {
		m_MoutionSpeed = 1.0f;
	}
	else {
		m_MoutionSpeed = m_MoutionSpeed_ComboAdd;
	}

	if (m_CurrentAttack.DamageType == DamageType::HighDamage) {
		createWeaponEffect(m_CurrentAttack.AttackTime * (1.0f / m_MoutionSpeed), WeaponEffectType::STRONGEF);
	}


	changeAnime(m_CurrentAttack.MoutionID);

	m_UseGravity = m_CurrentAttack.UseGravity;

	if (!m_UseGravity) {
		mJump.y = 0.0f;
	}

	m_NextAttack = -1;

	if (m_CurrentAttack.FloatMove != 0.0f) {
		mJump.y = 0.0f;
		mJump.y += m_CurrentAttack.FloatMove;
	}
}

void PlayerController::createWeaponEffect(float time, int type)
{
	if (auto w = GetWeapon()) {
		w->SetLifeTime(time);
		w->CreateEffect((WeaponEffectType)type);
	}
}

void PlayerController::freeAnimeUpdate()
{
	if (m_FreeAIMMode) {
		return;
	}

	WeaponType t = WeaponType::Count;
	if (auto w = GetWeapon()) {
		t = w->GetWeaponType();
	}

	if (!m_IsGround) {
		if (mJump.y > 0.0f) {
			changeAnime(AnimeID::Jump);
		}
		else {
			changeAnime(AnimeID::Fall);
		}
	}
	else {
		if (m_CurrentAnimeID == AnimeID::Fall) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Landing, gameObject->mTransform->WorldPosition());
			changeAnime(AnimeID::FallGround);
		}
		else {
			if (m_CurrentAnimeID == AnimeID::FallGround) {
				if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
					if (!anime->IsAnimationEnd(AnimeID::FallGround)) {
						return;
					}
				}
			}


			if (m_CurrentAnimeID == AnimeID::WeaponDrop) {
				if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
					if (!anime->IsAnimationEnd(AnimeID::WeaponDrop)) {
						return;
					}
				}
			}

			if (mJump.x == 0.0f && mJump.z == 0.0f) {
				if (t == WeaponType::Rance) {
					changeAnime(AnimeID::RIdle);
				}
				else {
					changeAnime(AnimeID::Idle);
				}
			}
			else {
				m_MoveSETimer += Hx::DeltaTime()->GetDeltaTime();
				if (m_MoveSETimer >= 0.5f) {
					m_MoveSETimer = 0.0f;
					SoundManager::PlaySE(SoundManager::SoundSE_ID::Player_Run2, gameObject->mTransform->WorldPosition());
				}

				if (t == WeaponType::Rance) {
					changeAnime(AnimeID::RMove);
				}
				else {
					changeAnime(AnimeID::Move);
				}
			}
		}
	}
}

float PlayerController::GetMovementSpeed()
{
	return m_MoveSpeed * m_MoveSpeed_ComboAdd;
}

#include <algorithm>
void PlayerController::changeAnime(int id)
{
	if (id == m_CurrentAnimeID)return;
	m_CurrentAnimeID_Stack = id;
	m_ChangeAnime = true;
}

float PlayerController::getMoutionTime(int id)
{
	if (!m_AnimeModel)return -1.0f;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return -1.0f;
	return anime->GetTotalTime(id) / 30.0f;
}

void PlayerController::animeFlip()
{
	if (!m_AnimeModel)return;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return;

	if (m_CurrentAnime_Weight != 1.0f) {
		m_CurrentAnime_Weight += 10.0f*Hx::DeltaTime()->GetDeltaTime() * m_MoutionSpeed;
		m_CurrentAnime_Weight = min(m_CurrentAnime_Weight, 1.0f);

		if (m_CurrentAnimeID == m_CurrentAnimeID_Back) {
			if (m_CurrentAnimeID >= 0) {
				auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
				p.mWeight = 1.0f;
				anime->SetAnimetionParam(m_CurrentAnimeID, p);
			}
		}
		else {
			if (m_CurrentAnimeID >= 0) {
				auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
				p.mWeight = m_CurrentAnime_Weight;
				anime->SetAnimetionParam(m_CurrentAnimeID, p);
			}
			if (m_CurrentAnimeID_Back >= 0) {
				auto p = anime->GetAnimetionParam(m_CurrentAnimeID_Back);
				p.mWeight = 1.0f - m_CurrentAnime_Weight;
				anime->SetAnimetionParam(m_CurrentAnimeID_Back, p);
			}
		}

	}

	if (m_CurrentAnimeID == m_CurrentAnimeID_Stack) {
		m_ChangeAnime = false;
	}

	if (!m_ChangeAnime)return;
	m_ChangeAnime = false;

	if (m_CurrentAnimeID_Back >= 0) {
		auto p = anime->GetAnimetionParam(m_CurrentAnimeID_Back);
		p.mWeight = 0.0f;
		p.mTime = 0.0f;
		p.mTimeScale = 0.0f;
		anime->SetAnimetionParam(m_CurrentAnimeID_Back, p);
	}

	m_CurrentAnime_Weight = 1.0f;

	if (m_CurrentAnimeID >= 0) {
		auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
		p.mWeight = 1.0f - m_CurrentAnime_Weight;
		//p.mTime = 0.0f;
		p.mTimeScale = 0.0f;
		anime->SetAnimetionParam(m_CurrentAnimeID, p);
	}
	auto p = anime->GetAnimetionParam(m_CurrentAnimeID_Stack);
	p.mTime = 0.0f;
	p.mWeight = m_CurrentAnime_Weight;
	p.mTimeScale = m_MoutionSpeed;
	anime->SetAnimetionParam(m_CurrentAnimeID_Stack, p);
	m_CurrentAnimeID_Back = m_CurrentAnimeID;
	m_CurrentAnimeID = m_CurrentAnimeID_Stack;
}

void PlayerController::stateFlip()
{
	if (m_PlayerState_Stack == m_PlayerState)return;
	if (m_PlayerState_Stack < 0)return;
	if (m_PlayerState_Stack >= PlayerState::Count)return;
	if (m_PlayerState >= 0) {
		m_StateFunc[m_PlayerState].Exit();
	}
	m_PlayerState = m_PlayerState_Stack;
	m_StateFunc[m_PlayerState].Enter();
}

void PlayerController::ComboAdvantage()
{
	m_ComboTimer -= Hx::DeltaTime()->GetDeltaTime();
	if (m_ComboTimer <= 0) {
		m_ComboTimer = 0.0f;
		m_HitCount = 0;
	}

	if (m_HitCount >= 0) {
		m_WeaponResist_ComboAdd = 1.0f;
		m_MoveSpeed_ComboAdd = 1.0f;
		m_MoutionSpeed_ComboAdd = 1.0f;
		m_ATK_ComboAdd = 1.0f;
	}
	if (m_HitCount >= 4) {
		m_MoutionSpeed_ComboAdd = 1.04f;
		m_MoveSpeed_ComboAdd = 1.04f;

	}
	if (m_HitCount >= 5) {
		m_ATK_ComboAdd = 1.05f;

	}
	if (m_HitCount >= 8) {
		m_MoutionSpeed_ComboAdd = 1.08f;
		m_MoveSpeed_ComboAdd = 1.08f;

	}
	if (m_HitCount >= 10) {
		m_WeaponResist_ComboAdd = 0.8f;
		m_MoveSpeed_ComboAdd = 1.5f;
		m_ATK_ComboAdd = 1.1f;
	}
	if (m_HitCount >= 12) {
		m_MoutionSpeed_ComboAdd = 1.12f;
		m_MoveSpeed_ComboAdd = 1.12f;

	}
	if (m_HitCount >= 15) {
		m_ATK_ComboAdd = 1.15f;

	}
	if (m_HitCount >= 16) {
		m_MoutionSpeed_ComboAdd = 1.16f;
		m_MoveSpeed_ComboAdd = 1.16f;

	}
	if (m_HitCount >= 20) {
		m_WeaponResist_ComboAdd = 0.4f;
		m_MoutionSpeed_ComboAdd = 1.2f;
		m_MoveSpeed_ComboAdd = 1.2f;
		m_ATK_ComboAdd = 1.2f;
	}
}

void PlayerController::AddCombo()
{
	m_HitCount++;
	if (m_HitCount >= 20) {
		m_ComboTimer = 10.0f;
	}else if (m_HitCount >= 10) {
		m_ComboTimer = 7.5f;
	}
	else if (m_HitCount >= 0) {
		m_ComboTimer = 5.0f;
	}
}

void PlayerController::ClearCombo()
{
	m_HitCount=0;
	m_ComboTimer = 1.0f;
}


void PlayerController::BoneMoveUpdate()
{
	if (m_AnimeModel&&m_BoneMirrorObject) {
		auto model = m_AnimeModel->mTransform->WorldPosition();
		auto bone = m_BoneMirrorObject->mTransform->WorldPosition();

		bool flag = false;
		if (XMVector3Length(m_BoneMirrorObject->mTransform->Position()).x > 0.01f) {
			flag = true;
		}
		bone -= model;
		//bone.y = 0.0f;
		//m_AnimeModel->mTransform->Position(m_BoneMirrorObject->mTransform->Position()*0.02);
		if (flag) {
			auto move = bone - m_BoneBackPos;

			if (m_CharacterControllerComponent) {
				m_CharacterControllerComponent->Move(move);
			}
		}

		m_BoneBackPos = bone;
	}
}
