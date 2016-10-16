#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Sandbag.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Weapon::Initialize(){
	m_Endurance = 1;
	m_AttackForce = 1;
	m_Recast = 1;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Weapon::Start(){
}

//毎フレーム呼ばれます
void Weapon::Update(){
	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Weapon::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Weapon::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Weapon::OnCollideEnter(GameObject target){
	(void)target;
	if (target->GetLayer() == 3){
		if (auto scr = target->GetScript<Sandbag>()) {
			if (m_Recast > 1.0f) {
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

	gameObject->GetComponent<PhysXComponent>()->SetGravity(true);

	XMVECTOR wpos = gameObject->mTransform->WorldPosition();
	gameObject->mTransform->SetParent(Hx::GetRootActor());
	gameObject->mTransform->WorldPosition(wpos);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(false);

}
/// <summary>
///武器を捨てる処理
/// </summary>
void Weapon::ThrowAway(XMVECTOR & throwdir)
{
	ThrowAway();
	gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir, ForceMode::eIMPULSE);
}
/// <summary>
///武器を拾うときに必ず呼ぶ
///とりあえずトリガーをonにする。
/// </summary>
void Weapon::GetWeapon()
{
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
}
