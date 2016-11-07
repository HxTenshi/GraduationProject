#include "PlayerController.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "WeaponHand.h"
# include "MoveAbility.h"
#include "TPSCamera.h"
#include "GetWeapon.h"
#include "TimeManager.h"
#include "GetEnemy.h"
# include "AimController.h"

struct AttackID {
	enum Enum {
		Low1,
		Low2,
		Low3,
		High1,
		High2,
		FloatLow1,
		Special,
		Count,
	};
};

struct AnimeID {
	enum Enum {
		Idle,
		Move,
		AttackLow1,
		Guard,
		Dogde,
		KnockBack,
		Count,
	};
};

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
	mVelocity = XMVectorZero();
	m_IsGround = false;
	m_NextAttack = -1;
	m_AttackMode = false;
	m_LockOnEnabled = false;
	m_InvisibleTime = 0.0f;
	m_IsInvisible = false;
	m_IsDead = false;
	m_IsGuard = false;

	m_DodgeTimer = 0.0f;
	m_MoveVelo = XMVectorZero();

	m_StateFunc.resize(PlayerState::Count);

#define AddFunc(x) \
	m_StateFunc[PlayerState::x].Enter	= std::bind(&PlayerController::x##Enter, this);\
	m_StateFunc[PlayerState::x].Excute	= std::bind(&PlayerController::x##Excute, this);\
	m_StateFunc[PlayerState::x].Exit	= std::bind(&PlayerController::x##Exit, this);

	AddFunc(Lock);
	AddFunc(Free);
	AddFunc(Dodge);
	AddFunc(Attack);
	AddFunc(KnockBack);
	AddFunc(Down);
	AddFunc(Movie);

	m_PlayerState = (PlayerState::Enum)-1;
	SetPlayerState(PlayerState::Lock);

	m_CurrentAnimeID = -1;

	m_AttackStateList.resize(AttackID::Count);


	AttackState attack;

	attack.ID = AttackID::Low1;
	attack.NextLowID = AttackID::Low2;
	attack.NextHighID = AttackID::High2;
	attack.MoutionID = AnimeID::AttackLow1;
	attack.AddSpecial = 2.0f;

	attack.KoutyokuTime = 0.5f;
	attack.NextTime = 0.5f;
	attack.DamageScale = 1.0f;
	attack.AttackTime = 1.0f;
	attack.AttackFunc = [&](){
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->LowAttack_1();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::Low2;
	attack.NextLowID = AttackID::Low3;
	attack.NextHighID = AttackID::High2;
	attack.AttackFunc = [&]() {
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->LowAttack_2();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::Low3;
	attack.NextLowID = -1;
	attack.NextHighID = AttackID::High2;
	attack.AttackFunc = [&]() {
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->LowAttack_3();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::High1;
	attack.NextLowID = -1;
	attack.NextHighID = AttackID::High2;
	attack.AttackFunc = [&]() {
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->HighAttack_1();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::High2;
	attack.NextLowID = -1;
	attack.NextHighID = -1;
	attack.AttackFunc = [&]() {
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->HighAttack_2();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::FloatLow1;
	attack.NextLowID = -1;
	attack.NextHighID = -1;
	attack.AttackFunc = [&](){
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->FloatLowAttack_1();
		}
	};
	m_AttackStateList[attack.ID] = attack;

	attack.ID = AttackID::Special;
	attack.NextLowID = -1;
	attack.NextHighID = -1;
	attack.AttackFunc = [&]() {
		auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
		if (weaponHand) {
			weaponHand->SpecialAttack();
		}
	};
	m_AttackStateList[attack.ID] = attack;
	

	m_CurrentAttack.ID = -1;
	m_CurrentAttack.NextLowID = -1;
	m_CurrentAttack.NextHighID = -1;
	m_CurrentAttack.MoutionID = 0;
	m_CurrentAttack.KoutyokuTime = 0.0f;
	m_CurrentAttack.NextTime = 0.0f;
	m_CurrentAttack.DamageScale = 0.0f;
	m_CurrentAttack.AttackTime = 0.0f;
	m_CurrentAttack.AddSpecial = 0.0f;
	m_CurrentAttack.AttackFunc = [](){};

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerController::Start(){

	m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
}

//毎フレーム呼ばれます
void PlayerController::Update(){


	if (!m_CharacterControllerComponent) {
		m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
		if (!m_CharacterControllerComponent)return;
	}

	m_StateFunc[m_PlayerState].Excute();

	animeFlip();

	if (Input::Down(KeyCode::Key_O)) {
		Damage(5.0f,XMVectorSet(0,0,-1,1),KnockBack::Low);
	}
	if (Input::Down(KeyCode::Key_P)) {
		Damage(10.0f, XMVectorSet(0, 0, -1, 1), KnockBack::Down);
	}

	if (!m_WeaponHand)return;

	if (Input::Down(KeyCode::Key_E)) {
		throwAway();
	}

	//10 / 29 更新
	if (mMoveAvility) {
		if (Input::Down(KeyCode::Key_Q)) {
			//敵のターゲット取得処理
			if (!m_Camera)return;
			auto camera = m_Camera->GetScript<TPSCamera>();
			if (camera) {
				GameObject target = camera->GetLookTarget();
				//ロックオンしている敵が居たら投げれる
				if (target) {
					throwAway(target);
					if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
						script->SetPoint(target, m_CharacterControllerComponent);
					}
				}
			}
		}

		if (Input::Down(KeyCode::Key_O)) {
			if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
				script->OnMove();
			}
		}

		auto camera = m_Camera->GetScript<TPSCamera>();

		if (camera) {
			if (auto aim = mAimController->GetScript<AimController>()) {
				if (Input::Down(KeyCode::Key_I)) {
					aim->ChangeAimMode(camera, gameObject, true);

				}
				if (Input::Up(KeyCode::Key_I)) {
					aim->ChangeAimMode(camera, gameObject, false);
					Hx::Debug()->Log("UP : "+std::to_string(camera->gameObject->mTransform->Forward().x));
					Hx::Debug()->Log("UP : " + std::to_string(camera->gameObject->mTransform->Forward().y));
					Hx::Debug()->Log("UP : " + std::to_string(camera->gameObject->mTransform->Forward().z));
					auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
					if (weaponHand) {
						weaponHand->ThrowAway(camera->gameObject->mTransform->Forward());
					}
					//throwAway(camera->gameObject->mTransform->WorldPosition() * -1, true);
				}
			}
		}





	}
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
	if (state < 0)return;
	if (state >= PlayerState::Count)return;
	if (m_PlayerState >= 0) {
		m_StateFunc[m_PlayerState].Exit();
	}
	m_PlayerState = state;
	m_StateFunc[m_PlayerState].Enter();
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
		m_InvisibleTime = 0.5f;
		mVelocity = attackVect;
		SetPlayerState(PlayerState::KnockBack);
		break;
	case PlayerController::KnockBack::Down:
		m_InvisibleTime = 2.0f;
		mVelocity = attackVect;
		SetPlayerState(PlayerState::Down);
		break;
	default:
		break;
	}
}

void PlayerController::Damage(float damage, const XMVECTOR& attackVect, KnockBack::Enum knockBackLevel)
{
	if (IsDogde()) {
		return;
	}
	if (IsGuard() && XMVector3Dot(mVelocity, attackVect).x > 0) {
		return;
	}
	AddHP(-damage);
	PlayKnockBack(attackVect,knockBackLevel);
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
	m_SpecialPower += power;
	m_SpecialPower = min(max(m_SpecialPower,0.0f), m_SpecialPowerMax);
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
}

void PlayerController::LockEnter()
{
}

void PlayerController::LockExcute()
{
	SetPlayerState(PlayerState::Free);
	changeAnime(AnimeID::Idle);
}

void PlayerController::LockExit()
{
}

void PlayerController::FreeEnter()
{
}

void PlayerController::FreeExcute()
{
	move();

	moveUpdate();

	lockOn();

	rotate();

	//周りの武器の取得,選択関連
	GettingWeapon();

	if (dodge()) {
		SetPlayerState(PlayerState::Dodge);
		return;
	}


	if (!m_WeaponHand)return;
	if (Input::Down(MouseCode::Right)) {
		guard();
		changeAnime(AnimeID::Guard);
	}
	else {
		m_IsGuard = false;
		if (mVelocity.x == 0.0f&&mVelocity.y == 0.0f) {
			changeAnime(AnimeID::Idle);
		}
		else {
			changeAnime(AnimeID::Move);
		}
	}
	if (attack()) {
		SetPlayerState(PlayerState::Attack);
	}



	if (Input::Down(KeyCode::Key_K)) {
		auto v = mVelocity;
		v *= -1;
		v.y *= -1;
		PlayKnockBack(v, KnockBack::Low);
	}
	if (Input::Down(KeyCode::Key_L)) {
		auto v = mVelocity;
		v *= -1;
		v.y = 5	;
		PlayKnockBack(v, KnockBack::Down);
	}
}

void PlayerController::FreeExit()
{
	m_IsGuard = false;
}

void PlayerController::AttackEnter()
{
	m_CurrentAttack = m_AttackStateList[m_NextAttack];

	m_CurrentAttack.AttackFunc();

	changeAnime(m_CurrentAttack.MoutionID);

	m_AttackMode = true;
	m_NextAttack = -1;

	mJump = XMVectorZero();
}

void PlayerController::AttackExcute()
{
	moveUpdate();

	lockOn();

	
	if (m_NextAttack==-1) {
		if (Input::Trigger(KeyCode::Key_C)) {
			m_NextAttack = m_CurrentAttack.NextLowID;
		}
		if (Input::Trigger(KeyCode::Key_V)) {
			m_NextAttack = m_CurrentAttack.NextHighID;
		}
	}

	float time = Hx::DeltaTime()->GetDeltaTime();
	m_CurrentAttack.AttackTime -= time;
	if (m_CurrentAttack.AttackTime > 0.0f)return;


	if (m_NextAttack >= 0) {
		m_CurrentAttack = m_AttackStateList[m_NextAttack];

		m_CurrentAttack.AttackFunc();

		changeAnime(m_CurrentAttack.MoutionID);

		m_AttackMode = true;
		m_NextAttack = -1;

		return;
	}

	m_CurrentAttack.NextTime -= time;
	m_CurrentAttack.KoutyokuTime -= time;
	if (m_CurrentAttack.KoutyokuTime > 0.0f)return;
	m_AttackMode = false;

	if (m_CurrentAttack.NextTime > 0.0f)return;

	SetPlayerState(PlayerState::Free);
}

void PlayerController::AttackExit()
{

	m_CurrentAttack.ID = -1;
	m_CurrentAttack.NextLowID = -1;
	m_CurrentAttack.NextHighID = -1;
	m_CurrentAttack.MoutionID = 0;
	m_CurrentAttack.KoutyokuTime = 0.0f;
	m_CurrentAttack.NextTime = 0.0f;
	m_CurrentAttack.DamageScale = 0.0f;
	m_CurrentAttack.AttackTime = 0.0f;
	m_CurrentAttack.AddSpecial = 0.0f;
	m_CurrentAttack.AttackFunc = []() {};

	m_NextAttack = -1;
}

void PlayerController::DodgeEnter()
{
	auto v = mJump;
	if (abs(v.x) == 0 && abs(v.z) == 0) {
		v = gameObject->mTransform->Forward();
	}
	mJump = XMVectorZero();

	m_DodgeTimer = 1.0f;

	m_MoveVelo = XMVector3Normalize(v) * m_MoveSpeed * 2;
	mJump.y += m_JumpPower / 2.0f;
}

void PlayerController::DodgeExcute()
{
	moveUpdate();
	lockOn();

	m_DodgeTimer -= Hx::DeltaTime()->GetDeltaTime();

	if (m_DodgeTimer <= 0.0f) {
		SetPlayerState(PlayerState::Free);
	}

	changeAnime(AnimeID::Dogde);
}

void PlayerController::DodgeExit()
{
	m_DodgeTimer = 0.0f;
	m_MoveVelo = XMVectorZero();
}

void PlayerController::KnockBackEnter()
{
	m_IsInvisible = true;
	mJump.y += mVelocity.y;
	m_MoveVelo.y = 0.0f;
	m_MoveVelo.x = mVelocity.x;
	m_MoveVelo.z = mVelocity.z;
	mVelocity *= -1;
	mVelocity.y = 0.0f;
	rotate();

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
	mJump.y += mVelocity.y;
	m_MoveVelo.y = 0.0f;
	m_MoveVelo.x = mVelocity.x;
	m_MoveVelo.z = mVelocity.z;
	mVelocity *= -1;
	mVelocity.y = 0.0f;
	rotate();
}

void PlayerController::DownExcute()
{
	moveUpdate();

	lockOn();

	float time = Hx::DeltaTime()->GetDeltaTime();
	m_InvisibleTime -= time;
	if (m_InvisibleTime <= 0.0f) {
		SetPlayerState(PlayerState::Free);
	}
	changeAnime(AnimeID::Idle);
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

void PlayerController::move()
{

	float time = Hx::DeltaTime()->GetDeltaTime();
	float speed = m_MoveSpeed;
	float x = 0, y = 0;
	if (Input::Down(KeyCode::Key_W)) {
		y = 1.0f;
	}
	if (Input::Down(KeyCode::Key_S)) {
		y = -1.0f;
	}
	if (Input::Down(KeyCode::Key_D)) {
		x = 1.0f;
	}
	if (Input::Down(KeyCode::Key_A)) {
		x = -1.0f;
	}


	m_IsGround = m_CharacterControllerComponent->IsGround();

	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	if (m_IsGround) {
		auto d = XMVectorSet(0, -1, 0, 1);
		RaycastHit hit;
		if (Hx::PhysX()->RaycastHit(pos, d, 100.0f, &hit)) {
			auto dot = XMVector3Dot(hit.normal, XMVectorSet(0, 1, 0, 1)).x;
			auto angle = dot;

			auto deg = m_CharacterControllerComponent->GetSlopeLimit();
			float slopeLimit = cosf(XM_PI / 180.0f * deg);
			if (slopeLimit > angle) {
				m_IsGround = false;

				hit.normal.y = 0.0f;
				hit.normal = XMVector3Normalize(hit.normal);
				auto v2 = mJump + hit.normal * speed * 0.2f;
				v2.y = 0.0f;
				auto s = min(max(XMVector3Length(v2).x, -speed), speed);
				v2 = XMVector3Normalize(v2)*s;
				mJump.x = v2.x;
				mJump.z = v2.z;
			}
		}
	}

	auto xy = XMVector2Normalize(XMVectorSet(x, y, 0, 1));
	auto v = XMVectorZero();
	if (m_Camera) {
		v += xy.y * m_Camera->mTransform->Forward();
		v += xy.x * m_Camera->mTransform->Left();

		v.y = 0.0f;
		if(XMVector3Length(v).x != 0)
		v = XMVector3Normalize(v);
	}

	mVelocity = v;

	if (m_IsGround) {
		mJump = XMVectorZero();
		if (Input::Trigger(KeyCode::Key_SPACE)) {
			mJump.y += m_JumpPower;
		}
		v *= speed;

		mJump.x = v.x;
		mJump.z = v.z;
	}
	else{

		//mJump.x -= mJump.x * 6.0f * time;
		//mJump.z -= mJump.z * 6.0f * time;
		auto v2 = mJump + v * speed * 1.5f * time;
		v2.y = 0.0f;
		auto s = min(max(XMVector3Length(v2).x, -speed), speed);
		v2 = XMVector3Normalize(v2)*s;
		mJump.x = v2.x;
		mJump.z = v2.z;

	}

}

void PlayerController::moveUpdate()
{

	if (m_IsGround && !m_CharacterControllerComponent->IsGround() && mJump.y <= 0.0f) {
		XMVECTOR donw = XMVectorSet(0, -m_CharacterControllerComponent->GetStepOffset(), 0, 1);
		m_CharacterControllerComponent->Move(donw);
		if (!m_CharacterControllerComponent->IsGround()) {
			m_CharacterControllerComponent->Move(-donw);
		}
	}


	float time = Hx::DeltaTime()->GetDeltaTime();
	if (m_CharacterControllerComponent->IsGround()) {
		m_FloatJumpTimer = 0.0f;
	}

	if (m_FloatJumpTimer == 0.0f) {
		bool up = mJump.y > 0.0f;
		mJump += mGravity * time;
		up = up && mJump.y <= 0.0f;
		if (up) {
			m_FloatJumpTimer = 1.0f;
			mJump.y = 0.0f;
		}
	}
	else {
		m_FloatJumpTimer -= time;
		m_FloatJumpTimer = max(m_FloatJumpTimer, 0.0f);
	}

	auto p = XMVectorZero();
	p += mJump * time;
	p += m_MoveVelo * time;

	m_CharacterControllerComponent->Move(p);

	m_IsGround = m_CharacterControllerComponent->IsGround();

}

void PlayerController::rotate()
{
	if (XMVector3Length(mVelocity).x == 0)return;
	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0,0,0,1), mVelocity, XMVectorSet(0, 1, 0, 1)));
	auto q = XMQuaternionRotationMatrix(rotate);
	gameObject->mTransform->WorldQuaternion(q);
}

bool PlayerController::dodge()
{
	if (m_IsGround) {
		if (Input::Trigger(KeyCode::Key_LSHIFT)) {
			return true;
		}
	}

	return false;
}

#include "WeaponHand.h"
void PlayerController::guard()
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponHand) {
		weaponHand->Guard();
		m_IsGuard = true;
	}
}

bool PlayerController::attack()
{
	if (m_IsGround) {
		if (Input::Trigger(KeyCode::Key_C)) {
			m_NextAttack = AttackID::Low1;
		}
		if (Input::Trigger(KeyCode::Key_V)) {
			m_NextAttack = AttackID::High1;
		}
		if (Input::Trigger(KeyCode::Key_B)) {
			if (GetSpecial() >= m_SpecialPowerMax) {
				m_NextAttack = AttackID::Special;
				SetSpecial(0.0f);
			}
		}
	}
	else {
		if (Input::Trigger(KeyCode::Key_C)) {
			m_NextAttack = AttackID::FloatLow1;
		}
	}
	return (m_NextAttack != -1);
}

void PlayerController::throwAway(GameObject target,bool isMove)
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (target && weaponHand) {
		weaponHand->ThrowAway(target,isMove);
	}
	else if (weaponHand) {
		weaponHand->ThrowAway();
	}
}


void PlayerController::lockOn()
{
	if (!m_Camera)return;
	auto camera = m_Camera->GetScript<TPSCamera>();

	if (camera) {
		camera->SetLeft(0.0f);


		if (m_LockOnEnabled) {
			if (auto target = camera->GetLookTarget()) {
				auto pos = gameObject->mTransform->WorldPosition();
				auto tarpos = target->mTransform->WorldPosition();
				if (XMVector3Length(pos - tarpos).x > 50.0f) {
					m_LockOnEnabled = false;
					camera->SetLookTarget(NULL);
				}
			}
			else {
				m_LockOnEnabled = false;
			}
		}
	}

	if (Input::Trigger(KeyCode::Key_T)) {

		if (!m_LockOnEnabled) {
			if (!m_GetEnemy)return;
			auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

			if (!getenemy)return;
			auto enemy = getenemy->GetMinEnemy();
			if (!enemy)return;

			if (camera) {
				camera->SetLookTarget(enemy);
			}
		}
		else {
			camera->SetLookTarget(NULL);
		}
	
		m_LockOnEnabled = !m_LockOnEnabled;
	}

	if (!m_LockOnEnabled)return;
	
	if (Input::Trigger(KeyCode::Key_Y)) {

		if (!m_GetEnemy)return;
		auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

		if (!getenemy)return;
		if (camera) {

			auto enemy = getenemy->GetPointMinEnemy(camera->GetLookTarget(), GetEnemy::MinVect::left);
			if (!enemy)return;
			camera->SetLookTarget(enemy);
		}

	}
	if (Input::Trigger(KeyCode::Key_U)) {

		if (!m_GetEnemy)return;
		auto getenemy = m_GetEnemy->GetScript<GetEnemy>();

		if (!getenemy)return;
		if (camera) {

			auto enemy = getenemy->GetPointMinEnemy(camera->GetLookTarget(), GetEnemy::MinVect::right);
			if (!enemy)return;
			camera->SetLookTarget(enemy);
		}

	}

	auto f = m_Camera->mTransform->Forward();
	f.y = 0.0f;
	f = XMVector3Normalize(f);
	mVelocity = f;

	auto left = m_Camera->mTransform->Left();
	auto v = mJump;
	v.y = 0.0f;
	v = XMVector3Normalize(v);
	float dot = XMVector3Dot(v,left).x;

	if (abs(dot) > 0.5f) {
		float l = dot / abs(dot);
		if (camera) {
			camera->SetLeft(l * 2.0f);
		}
	}

}

void PlayerController::GettingWeapon(){

	//早期リターン
	if (!m_WeaponHand) return;
	if (!m_GetWeapon) return;
	if (!m_TimeManager) return;

	//GetWeaponのスクリプトの取得
	auto getWeapon = m_GetWeapon->GetScript<GetWeapon>();
	if (!getWeapon) return;
	
	//WeaponHandのスクリプトの取得
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (!weaponHand) return;

	//TimeManagerのスクリプトの取得
	auto timeMgr = m_TimeManager->GetScript<TimeManager>();
	if (!timeMgr) return;

	if (!m_marker) return;

	if (weaponHand->GetHandWeapon()) {
		m_marker->Disable();
		return;
	}

	if (m_tempWeapon) {
		m_marker->Enable();
		XMVECTOR markerPos = m_tempWeapon->mTransform->WorldPosition() + XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_marker->mTransform->WorldPosition(markerPos);
	}
	else {
		m_marker->Disable();
	}

	//一定時間長押ししたら
	if (m_InputF_Time > 0.5f) {
		//スローモードにする
		timeMgr->OnSlow();

		if (Input::Trigger(KeyCode::Key_G)) {
			//選択対象から左に一番近いものを取得
			auto t = getWeapon->GetPointMinWeapon(m_tempWeapon, GetWeapon::MinVect::left);
			if(t)
			m_tempWeapon = t;

		}
		else if (Input::Trigger(KeyCode::Key_H)) {
			//選択対象から右に一番近いものを取得
			auto t = getWeapon->GetPointMinWeapon(m_tempWeapon, GetWeapon::MinVect::right);
			if (t)
				m_tempWeapon = t;
		}
	}

	//Fキーを押している時間をカウント
	if (Input::Down(KeyCode::Key_F)) {
		m_InputF_Time += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	}
	//Fキーを話したら
	else if (Input::Up(KeyCode::Key_F)) {
		if (m_tempWeapon) { 
		//選択した武器をセット
			weaponHand->SetWeapon(m_tempWeapon, [&](auto o, auto t) {
					if (t == Weapon::HitState::Damage) {
						AddSpecial(m_CurrentAttack.AddSpecial);
					}
				}
			);
		}
		//カウントをリセット
		m_InputF_Time = 0.0f;
		//スローモード解除
		timeMgr->OffSlow();
	}
	else {
		//常に一番近い武器を取得
		m_tempWeapon = getWeapon->GetMinWeapon();
	}
}

#include <algorithm>
void PlayerController::changeAnime(int id)
{
	m_CurrentAnimeID_Stack = id;
}

void PlayerController::animeFlip()
{
	if (!m_AnimeModel)return;
	if (m_CurrentAnimeID_Stack == m_CurrentAnimeID)return;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return;
	if (m_CurrentAnimeID >= 0) {
		auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
		p.mWeight = 0.0f;
		anime->SetAnimetionParam(m_CurrentAnimeID, p);
	}
	auto p = anime->GetAnimetionParam(m_CurrentAnimeID_Stack);
	p.mTime = 0.0f;
	p.mWeight = 1.0f;
	anime->SetAnimetionParam(m_CurrentAnimeID_Stack, p);
	m_CurrentAnimeID = m_CurrentAnimeID_Stack;
}
