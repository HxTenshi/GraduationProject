#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Enemy.h"
#include "WeaponEffect.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Weapon::Initialize(){
	is_hand = false;
	is_fly = false;
	is_attack = 0;
	m_weapon_rot = 0.0f;
	break_time = 0.0f;
	is_ground_hit = true;
	mIsEnemyThrow = false;
	m_ThrowTarget = NULL;
	m_MirrorTarget = NULL;
	m_ThrowSpeed = 1.0f;
	m_Vector = XMVectorZero();
		m_param.SetAttack(5);
		m_param.SetDurableDamage(2.0f,3.0f);
		m_param.SetDurable(400);
		m_param.SetName("DebugWeapon");
		m_param.SetWeaponType(WeaponType::Sword);
	
	SetHitCollback([](auto o,auto w, auto t) {});
	//auto child = gameObject->mTransform->Children();
	//for (auto it = child.begin(); it != child.end(); ++it) {
	//	if (it->Get()->Name == "hit") {
	//		m_ThrowHit = it->Get()->mTransform->gameObject;
	//	}
	//}

	mWeaponControl = Hx::FindActor("WeaponControl");
	//m_ThrowHit = 
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Weapon::Start(){

	auto m_table = WeaponTable::GetWeaponTable();
	if (m_table) {
		m_param = m_table->GetWeaponParametor(m_name);
	}
	//m_param.DebugLog();
}

//毎フレーム呼ばれます
void Weapon::Update(){	

	if (m_param.isBreak() & !is_hand) {
		auto g = Hx::Instance(m_break_effect);
		if (!g)Hx::DestroyObject(gameObject);
		auto pos = gameObject->mTransform->WorldPosition();
		g->mTransform->WorldPosition(pos);
		Hx::DestroyObject(gameObject);
		return;
	}
	if (is_attack == 2) {
		is_attack = 3;
	}
	BreakWeapon();
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
	ThrowAwayAction();
	m_weapon_rot += Hx::DeltaTime()->GetDeltaTime()*10.0f;

	{
		//ここで対象の敵の紐付け
		auto mirrer = gameObject->GetComponent<BoneMirrorComponent>();
		if (!mirrer)return;
		if (!mirrer->IsEnabled())return;
		if (!m_MirrorTarget) {
			m_MirrorTarget = NULL;
			mirrer->Disable();
			ThrowAway();
		}
	}
	//PierceSupport(gameObject);
	//Hx::Debug()->Log(std::to_string(gameObject->mTransform->DegreeRotate().x));

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Weapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Weapon::OnCollideBegin(GameObject target){
	//投げたときだけの制御
	if (!target)return;

	if (mIsEnemyThrow) {
		if (mWeaponControl) {
			if (target->GetLayer() == 3) {
				if (auto weapon = mWeaponControl->GetScript<WeaponControl>()) {
					weapon->HitActor(target, gameObject);
				}

				if (target->Name() == "gateCollider") {
					WeaponUsePhysX();
				}

				m_Recast = 0.0f;
				//scr->Damage(m_AttackForce);
				m_HitCollback(target, this, HitState::Damage);
				
				is_attack = 0;
				mIsEnemyThrow = false;
				m_ThrowTarget = NULL;
				SetHitCollback([](auto o, auto w, auto t) {});
			}

			if (target->GetLayer() == 4) {
				//Hx::Debug()->Log("Stageに当たった");
				if (auto weapon = mWeaponControl->GetScript<WeaponControl>()) {
					//Hx::Debug()->Log("移動可能");
					WeaponUsePhysX();
					weapon->Hit();
					//weapon->HitStage(target, gameObject, gameObject->GetComponent<PhysXComponent>());
				}
				is_attack = 0;
				mIsEnemyThrow = false;
				m_ThrowTarget = NULL;
				SetHitCollback([](auto o, auto w, auto t) {});
			}

			//if (target->GetLayer() == 6) {
			//	WeaponUsePhysX();
			//
			//	//auto wpos = gameObject->mTransform->WorldPosition();
			//	//gameObject->mTransform->WorldPosition(wpos - m_Vector);
			//	//ThrowAway();
			//	mIsEnemyThrow = false;
			//	m_ThrowTarget = NULL;
			//	SetHitCollback([](auto o, auto w, auto t) {});
			//}

		}

	}

	if (target->GetLayer() == 3 && is_hand) {
		//敵へのダメージの処理
		if (auto scr = Enemy::GetEnemy(target)) {
			if (!is_ground_hit) {
				m_Recast = 0.0f;
				//scr->Damage(m_AttackForce);
				m_HitCollback(target,this,HitState::Damage);
			}
		}
	}
} 

//コライダーとのヒット中に呼ばれます
void Weapon::OnCollideEnter(GameObject target) {
	if (!target)return;
	if (is_attack == 1 || is_attack == 2) {
		if (target->GetLayer() == 3 && is_hand) {
			//敵へのダメージの処理
			if (auto scr = Enemy::GetEnemy(target)) {
				if (!is_ground_hit) {
					m_Recast = 0.0f;
					//scr->Damage(m_AttackForce);
					m_HitCollback(target, this, HitState::Damage);
				}
			}
		}
	}
	if (is_attack == 1) {
		is_attack = 2;
	}
	 
}

//コライダーとのロスト時に呼ばれます
void Weapon::OnCollideExit(GameObject target){
	(void)target;
}
/// <summary>
///武器へのダメージ
/// </summary>
void Weapon::Damage(DamageType type,float mag=1.0f)
{
	m_param.Damage(type,mag);
}
/// <summary>
///武器が壊れた時の判定
/// </summary>
bool Weapon::isBreak()
{
	return m_param.isBreak();
}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway()
{
	break_time = 0.0f;
	is_fly = true;
	is_hand = false;
	is_ground_hit = false;
	m_weapon_rot = 0.0f;
	gameObject->GetComponent<PhysXComponent>()->SetGravity(true);
	XMVECTOR wpos = gameObject->mTransform->WorldPosition();
	wpos.y -= 0.6f;
	gameObject->mTransform->SetParent(Hx::GetRootActor());
	gameObject->mTransform->WorldPosition(wpos);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
	//gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(0.0f,1.0f,0.0f,1.0f) * 10, ForceMode::eIMPULSE);

	m_Vector = XMVectorSet(0,-1,0,1);

	if (XMVector3Length(m_Vector).x != 0.0f) {
		auto m = gameObject->mTransform->GetMatrix();
		auto s = gameObject->mTransform->Scale();
		//auto fl = XMVector3Length(m.r[0]).x;
		//auto ul = XMVector3Length(m.r[1]).x;
		//auto ll = XMVector3Length(m.r[2]).x;
		auto f = XMVector3Normalize(m.r[0]);
		auto u = XMVector3Normalize(m_Vector);
		auto l = XMVector3Cross(f, u);
		f = XMVector3Cross(u, l);
		m.r[0] = XMVector3Normalize(f);// * fl;
		m.r[1] = XMVector3Normalize(u);// * ul;
		m.r[2] = XMVector3Normalize(l);// * ll;
		gameObject->mTransform->WorldMatrix(m);
		gameObject->mTransform->Scale(s);
	}
}
void Weapon::ThrowAttack()
{
	is_hand = false;
	is_ground_hit = true;
	ThrowAway();
	
}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway(XMVECTOR & throwdir)
{
	mIsEnemyThrow = true;
	ThrowAway();
	is_ground_hit = true;
	is_attack = 1;
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	//gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir, ForceMode::eIMPULSE);

	m_Vector = throwdir;
	m_ThrowSpeed = XMVector3Length(m_Vector).x;

	if (XMVector3Length(m_Vector).x != 0.0f) {
		auto m = gameObject->mTransform->GetMatrix();
		auto s = gameObject->mTransform->Scale();
		//auto fl = XMVector3Length(m.r[0]).x;
		//auto ul = XMVector3Length(m.r[1]).x;
		//auto ll = XMVector3Length(m.r[2]).x;
		auto f = XMVector3Normalize(m.r[0]);
		auto u = XMVector3Normalize(m_Vector);
		auto l = XMVector3Cross(f, u);
		f = XMVector3Cross(u, l);
		m.r[0] = XMVector3Normalize(f);// * fl;
		m.r[1] = XMVector3Normalize(u);// * ul;
		m.r[2] = XMVector3Normalize(l);// * ll;
		gameObject->mTransform->WorldMatrix(m);
		gameObject->mTransform->Scale(s);
	}
}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway(GameObject target, float speed)
{
	mIsEnemyThrow = true;
	ThrowAway();
	is_ground_hit = true;
	is_attack = 1;
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	//gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir, ForceMode::eIMPULSE);

	m_ThrowTarget = target;
	auto pos = m_ThrowTarget->mTransform->WorldPosition();
	pos.y += 1.0f;
	m_Vector = pos - gameObject->mTransform->WorldPosition();
	m_ThrowSpeed = speed;

	if (XMVector3Length(m_Vector).x != 0.0f) {
		auto m = gameObject->mTransform->GetMatrix();
		auto s = gameObject->mTransform->Scale();
		//auto fl = XMVector3Length(m.r[0]).x;
		//auto ul = XMVector3Length(m.r[1]).x;
		//auto ll = XMVector3Length(m.r[2]).x;
		auto f = XMVector3Normalize(m.r[0]);
		auto u = XMVector3Normalize(m_Vector);
		auto l = XMVector3Cross(f, u);
		f = XMVector3Cross(u, l);
		m.r[0] = XMVector3Normalize(f);// * fl;
		m.r[1] = XMVector3Normalize(u);// * ul;
		m.r[2] = XMVector3Normalize(l);// * ll;
		gameObject->mTransform->WorldMatrix(m);
		gameObject->mTransform->Scale(s);
	}
}
void Weapon::WeaponUsePhysX()
{
	if (!is_hand) {

		gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
		gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
		is_hand = false;
		is_ground_hit = true;
		is_fly = false;
		is_attack = 0;
		//Hx::Debug()->Log("Hit");
	}
}
/// <summary>
///攻撃時のコールバック関数追加
/// </summary>
void Weapon::SetHitCollback(const HitCollbackType & collback)
{
	m_HitCollback = collback;
}
/// <summary>
///武器を拾うときに必ず呼ぶ
///とりあえずトリガーをonにする。
/// </summary>
void Weapon::GetWeapon()
{
	break_time = 0.0f;
	mIsEnemyThrow = false;
	is_ground_hit = false;
	is_hand = true;
	is_fly = false;
	is_attack = 0;
	m_ThrowTarget = NULL;
	//gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
	gameObject->GetComponent<PhysXComponent>()->SetKinematic(false);
	//Hx::Debug()->Log("get");


	if (auto mirror = gameObject->GetComponent<BoneMirrorComponent>()) {
		mirror->ChangeTargetBone(NULL);
		mirror->SetTargetBoneID(-1);
		mirror->Disable();
		m_MirrorTarget = NULL;
	}
}

float Weapon::GetAttackPower()
{
	return (isBreak())?0.1f:m_param.AttackParam();
}

float Weapon::GetDurable()
{
	return m_param.GetDurable();
}

WeaponType Weapon::GetWeaponType()
{
	return m_param.GetWeaponType();
}
//予期せぬバグがあります
void Weapon::SwapWeapon(GameObject target)
{
	if (!is_hand)return;
	if (true)return;
	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	XMVECTOR localpos = gameObject->mTransform->Position();
	XMVECTOR rot = gameObject->mTransform->Rotate();
	GameObject parent = gameObject->mTransform->GetParent();


	gameObject->mTransform->WorldPosition(target->mTransform->WorldPosition());
	gameObject->mTransform->Rotate(target->mTransform->Rotate());
	
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
	is_hand = false;
	is_fly = false;
	is_ground_hit = true;
	gameObject->mTransform->SetParent(NULL);


	target->mTransform->Position(localpos);
	target->mTransform->Rotate(rot);
	target->mTransform->SetParent(parent);
	//effect

}

bool Weapon::isGetWeapon()
{
	return !is_fly;
}

float Weapon::GetMaxDurable()
{
	return m_param.GetMaxDurable();
}

bool Weapon::isAttack()
{
	return is_attack != 0;
}

void Weapon::SetAttackFlag(bool flag)
{
	if (flag) {
		if (is_attack == 0) {
			is_attack = 1;
			if (m_WeaponEffect) {
				auto scr = m_WeaponEffect->GetScript<WeaponEffect>();
				scr->Action();
			}
		}
	}
	else {
		is_attack = 0;
	}
}

void Weapon::SetAttackFlag(int flag)
{
	SetAttackFlag((bool)flag);
}

bool Weapon::GetIsHand()
{
	return is_hand;
}

bool Weapon::isThrow()
{
	return mIsEnemyThrow;
}

void Weapon::SetMirrorTarget(GameObject target)
{
	m_MirrorTarget = target;
}
GameObject Weapon::GetMirrorTarget()
{
	return m_MirrorTarget;
}
std::string Weapon::GetName()
{
	return m_name;
}

void Weapon::ThrowAwayAction()
{
	if (mIsEnemyThrow) {
		if (m_ThrowTarget) {
			auto pos = m_ThrowTarget->mTransform->WorldPosition();
			pos.y += 1.0f;
			m_Vector = pos - gameObject->mTransform->WorldPosition();
		}
		auto pos = gameObject->mTransform->WorldPosition();
		pos += XMVector3Normalize(m_Vector) * m_ThrowSpeed * Hx::DeltaTime()->GetDeltaTime();
		gameObject->mTransform->WorldPosition(pos);
	}

	if (is_ground_hit || is_hand)return;
	m_weapon_rot = max(m_weapon_rot, 0);
	auto rot = gameObject->mTransform->WorldQuaternion();
	gameObject->mTransform->Rotate(XMVectorSet((m_weapon_rot*900 / 180.0f)*XM_PI, 0.0f, 0.0f, 1.0f));
}

void Weapon::PierceSupport(GameObject obj)
{
	if (!is_hand&&!is_ground_hit)return;
	XMVECTOR rot = obj->mTransform->DegreeRotate();
	if (rot.x<120&&rot.x>-120) {
		//obj->mTransform->DegreeRotate(XMVectorSet(180,0,0,1));
	}
}
//自分自身を消す
void Weapon::BreakWeapon()
{
	if (is_break_weapon) {
		//Hx::Debug()->Log("isbreakweapon");
		if (!is_hand) {
			break_time += Hx::DeltaTime()->GetDeltaTime();
			//Hx::Debug()->Log("ishand");
			if (break_time > weapon_break) {
				Hx::DestroyObject(gameObject);
				//Hx::Debug()->Log("break");
			}
		}
	}
}

void Weapon::Effect()
{
}