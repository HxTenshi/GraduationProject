#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Sandbag.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Weapon::Initialize(){
	is_hand = false;
	m_weapon_rot = 0.0f;
	is_ground_hit = true;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Weapon::Start(){
}

//毎フレーム呼ばれます
void Weapon::Update(){	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
	ThrowAwayAction();
	m_weapon_rot += Hx::DeltaTime()->GetDeltaTime();
	//PierceSupport(gameObject);
	//Hx::Debug()->Log(std::to_string(gameObject->mTransform->DegreeRotate().x));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Weapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Weapon::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Weapon::OnCollideEnter(GameObject target) {
	(void)target;
	if (target->GetLayer() == 3&is_hand){
		//サンドバッグへのダメージの処理
		if (auto scr = target->GetScript<Sandbag>()) {
			if (m_Recast > 1.0f&&!is_ground_hit) {
				m_Recast = 0.0f;
				scr->Damage(m_AttackForce);
			}
		}
	}

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
	m_Endurance -= damage;
}
/// <summary>
///武器が壊れた時の判定
/// </summary>
bool Weapon::isBreak()
{
	return (m_Endurance <= 0) ? true : false;
}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway()
{
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
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway(XMVECTOR & throwdir)
{
	ThrowAway();
	gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir*30.0f, ForceMode::eIMPULSE);
}
void Weapon::WeaponUsePhysX()
{
	if (!is_hand) {
		gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
		gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
		is_hand = false;
		is_ground_hit = true;
		Hx::Debug()->Log("Hit");
	}
}
/// <summary>
///武器を拾うときに必ず呼ぶ
///とりあえずトリガーをonにする。
/// </summary>
void Weapon::GetWeapon()
{
	is_hand = true;
	//gameObject->GetComponent<PhysXComponent>()->SetKinematic(true);
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
	gameObject->GetComponent<PhysXComponent>()->SetKinematic(false);
	Hx::Debug()->Log("get");
}

void Weapon::ThrowAwayAction()
{
	if (is_ground_hit)return;
	m_weapon_rot = max(m_weapon_rot, 0);
	auto rot = gameObject->mTransform->WorldQuaternion();
	gameObject->mTransform->Rotate(XMVectorSet((m_weapon_rot*450 / 180.0f)*XM_PI, 0.0f, 0.0f, 1.0f));
}

void Weapon::PierceSupport(GameObject obj)
{
	if (!is_hand&&!is_ground_hit)return;
	XMVECTOR rot = obj->mTransform->DegreeRotate();
	if (rot.x<120&&rot.x>-120) {
		//obj->mTransform->DegreeRotate(XMVectorSet(180,0,0,1));
	}
}