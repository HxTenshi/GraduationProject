#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Enemy.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Weapon::Initialize(){
	is_hand = false;
	is_fly = false;
	m_weapon_rot = 0.0f;
	is_ground_hit = true;
	mIsEnemyThrow = false;
	m_param.SetAttack(5);
	m_param.SetDurable(10);
	m_param.SetName("uhuuuu");
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
}

//毎フレーム呼ばれます
void Weapon::Update(){	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
	ThrowAwayAction();
	m_weapon_rot += Hx::DeltaTime()->GetDeltaTime()*10.0f;
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
			if (target->GetLayer() == 3)
				if (auto weapon = mWeaponControl->GetScript<WeaponControl>()) {
					Hx::Debug()->Log("敵に投げて当たった");
					Hx::Debug()->Log(target->Name());
					Hx::Debug()->Log(gameObject->Name());
					weapon->HitActor(target, gameObject);
				}
		}
	}
	if (target->GetLayer() == 3 && is_hand) {
		//サンドバッグへのダメージの処理
		if (auto scr = target->GetScript<Enemy>()) {
			if (!is_ground_hit) {
				m_Recast = 0.0f;
				//scr->Damage(m_AttackForce);
				m_HitCollback(target,this,HitState::Damage);
			}
		}
	}
	mIsEnemyThrow = false;
} 

//コライダーとのヒット中に呼ばれます
void Weapon::OnCollideEnter(GameObject target) {
	(void)target;
	 
}

//コライダーとのロスト時に呼ばれます
void Weapon::OnCollideExit(GameObject target){
	(void)target;
}
/// <summary>
///武器へのダメージ
/// </summary>
void Weapon::Damage(int damage)
{
	m_param.Damage(damage);
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
	SetHitCollback([](auto o,auto w,auto t) {});
	is_fly = true;
	is_hand = false;
	is_ground_hit = false;
	m_weapon_rot = 0.0f;
	gameObject->GetComponent<PhysXComponent>()->SetGravity(true);
	XMVECTOR wpos = gameObject->mTransform->WorldPosition();
	gameObject->mTransform->SetParent(Hx::GetRootActor());
	gameObject->mTransform->WorldPosition(wpos);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
	gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(0.0f,1.0f,0.0f,1.0f) * 10, ForceMode::eIMPULSE);
}
void Weapon::ThrowAttack()
{
	ThrowAway();
	is_hand = false;
	is_ground_hit = true;
}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway(XMVECTOR & throwdir)
{
	mIsEnemyThrow = true;
	ThrowAway();
	is_ground_hit = true;
	gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir*50.0f, ForceMode::eIMPULSE);
}
void Weapon::WeaponUsePhysX()
{
	if (!is_hand) {

		gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
		gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
		is_hand = false;
		is_ground_hit = true;
		is_fly = false;
		Hx::Debug()->Log("Hit");
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
	is_ground_hit = false;
	is_hand = true;
	is_fly = false;
	//gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
	gameObject->GetComponent<PhysXComponent>()->SetKinematic(false);
	Hx::Debug()->Log("get");
}

float Weapon::GetAttackPower()
{
	return (isBreak())?1.0f:m_param.AttackParam();
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

void Weapon::ThrowAwayAction()
{
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

void Weapon::Effect()
{

}
