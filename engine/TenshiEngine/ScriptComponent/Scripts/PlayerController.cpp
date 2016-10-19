#include "PlayerController.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "WeaponHand.h"
# include "MoveAbility.h"

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


//生成時に呼ばれます（エディター中も呼ばれます）
void PlayerController::Initialize(){
	mJump = XMVectorZero();
	mGravity = XMVectorSet(0, -9.81f, 0,1);
	mVelocity = XMVectorZero();
	m_IsDoge = false;
	m_IsGround = false;
	m_NextAttack = -1;
	m_AttackMode = false;

	m_AttackStateList.resize(AttackID::Count);


	AttackState attack;

	attack.ID = AttackID::Low1;
	attack.NextLowID = AttackID::Low2;
	attack.NextHighID = AttackID::High2;

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
	

	m_CullentAttack.ID = -1;
	m_CullentAttack.NextLowID = -1;
	m_CullentAttack.NextHighID = -1;
	m_CullentAttack.KoutyokuTime = 0.0f;
	m_CullentAttack.NextTime = 0.0f;
	m_CullentAttack.DamageScale = 0.0f;
	m_CullentAttack.AttackTime = 0.0f;
	m_CullentAttack.AttackFunc = [](){};

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void PlayerController::Start(){

	m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();

}

//毎フレーム呼ばれます
void PlayerController::Update(){

	if (!mMoveAvility) {
		return;
	}

	if (!m_CharacterControllerComponent) {
		m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
		if (!m_CharacterControllerComponent)return;
	}

	if (m_CharacterControllerComponent->IsGround()) {
		m_IsGround = true;
		m_IsDoge = false;
	}

	if(!m_IsDoge)
		move();

	moveUpdate();

	if (m_IsGround) {
		doge();
	}

	rotate();

	if (!m_WeaponHand)return;
	if (Input::Down(MouseCoord::Right)) {
		guard();
	}
	if (true) {
		attack();
	}
	if (Input::Down(KeyCoord::Key_E)) {
		throwAway();
	}

	if (Input::Down(KeyCoord::Key_Q)) {
		//ここで必要なのは対象のオブジェクトを検索出来るクラス
		//今は適当
		GameObject target = Hx::FindActor("sandbag");
		throwAway(target);
		if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
			script->SetPoint(target, m_CharacterControllerComponent);
		}
	}

	if (Input::Down(KeyCoord::Key_C)) {
		if (auto script = mMoveAvility->GetScript<MoveAbility>()) {
			script->OnMove();
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

void PlayerController::move()
{

	float time = Hx::DeltaTime()->GetDeltaTime();
	float speed = m_MoveSpeed;
	float x = 0, y = 0;
	if (Input::Down(KeyCoord::Key_W)) {
		y = 1.0f;
	}
	if (Input::Down(KeyCoord::Key_S)) {
		y = -1.0f;
	}
	if (Input::Down(KeyCoord::Key_D)) {
		x = 1.0f;
	}
	if (Input::Down(KeyCoord::Key_A)) {
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
		if (Input::Trigger(KeyCoord::Key_SPACE)) {
			mJump.y += m_JumpPower;
		}
		v *= speed;
	}
	else {

		mJump.x -= mJump.x * 6.0f * time;
		mJump.z -= mJump.z * 6.0f * time;
		auto v2 = mJump + v * speed * 0.1f;
		v2.y = 0.0f;
		auto s = min(max(XMVector3Length(v2).x, -speed), speed);
		v = XMVector3Normalize(v2)*s;

	}

	mJump.x = v.x;
	mJump.z = v.z;

}

void PlayerController::moveUpdate()
{
	float time = Hx::DeltaTime()->GetDeltaTime();
	mJump += mGravity * time;
	auto p = XMVectorZero();
	p += mJump * time;

	m_CharacterControllerComponent->Move(p);

	if (m_IsGround && !m_CharacterControllerComponent->IsGround() && mJump.y <= 0.0f) {
		XMVECTOR donw = XMVectorSet(0, -m_CharacterControllerComponent->GetStepOffset(), 0, 1);
		m_CharacterControllerComponent->Move(donw);
		if (!m_CharacterControllerComponent->IsGround()) {
			m_CharacterControllerComponent->Move(-donw);
		}
	}
}

void PlayerController::rotate()
{
	if (XMVector3Length(mVelocity).x == 0)return;
	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0,0,0,1), mVelocity, XMVectorSet(0, 1, 0, 1)));
	auto q = XMQuaternionRotationMatrix(rotate);
	gameObject->mTransform->WorldQuaternion(q);
}

void PlayerController::doge()
{
	if (Input::Trigger(KeyCoord::Key_LSHIFT)) {

		if (abs(mVelocity.x) == 0 && abs(mVelocity.z) == 0) {
			return;
		}

		mJump += mVelocity * m_MoveSpeed * 2;
		mJump.y += m_JumpPower/2.0f;
		auto p = mJump * Hx::DeltaTime()->GetDeltaTime();
		m_CharacterControllerComponent->Move(p);
		m_IsDoge = true;
	}
}

#include "WeaponHand.h"
void PlayerController::guard()
{
	auto weaponHand = m_WeaponHand->GetScript<WeaponHand>();
	if (weaponHand) {
		weaponHand->Guard();
	}
}

void PlayerController::attack()
{


	if (m_NextAttack == -1) {
		if (m_CullentAttack.NextTime > 0.0f) {
			if (Input::Trigger(KeyCoord::Key_C)) {
				m_NextAttack = m_CullentAttack.NextLowID;
			}
			if (Input::Trigger(KeyCoord::Key_V)) {
				m_NextAttack = m_CullentAttack.NextHighID;
			}
		}
		else {
			if (m_IsGround) {
				if (Input::Trigger(KeyCoord::Key_C)) {
					m_NextAttack = AttackID::Low1;
				}
				if (Input::Trigger(KeyCoord::Key_V)) {
					m_NextAttack = AttackID::High1;
				}
			}
			else {
				if (Input::Trigger(KeyCoord::Key_C)) {
					m_NextAttack = AttackID::FloatLow1;
				}
			}
		}

	}



	float time = Hx::DeltaTime()->GetDeltaTime();
	m_CullentAttack.AttackTime -= time;
	if (m_CullentAttack.AttackTime > 0.0f)return;


	if (m_NextAttack>=0) {
		m_CullentAttack = m_AttackStateList[m_NextAttack];

		m_CullentAttack.AttackFunc();

		m_AttackMode = true;
		m_NextAttack = -1;

		return;
	}

	m_CullentAttack.NextTime -= time;
	m_CullentAttack.KoutyokuTime -= time;
	if (m_CullentAttack.KoutyokuTime > 0.0f)return;
	m_AttackMode = false;

	if (m_CullentAttack.NextTime > 0.0f)return;

	m_NextAttack = -1;
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
