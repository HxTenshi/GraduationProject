#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void Weapon::Initialize(){
	m_Endurance = 1;
	m_frame_pos = gameObject->mTransform->WorldPosition();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Weapon::Start(){
	m_frame_pos = gameObject->mTransform->WorldPosition();
}

//毎フレーム呼ばれます
void Weapon::Update(){


	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	m_throw_dir.x = (m_frame_pos.x - pos.x)*100.0f;
	m_throw_dir.y = 3.0f;
	m_throw_dir.z = (m_frame_pos.z - pos.z)*100.0f;
	m_frame_pos= gameObject->mTransform->WorldPosition();
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
	gameObject->GetComponent<PhysXComponent>()->AddForce(m_throw_dir, ForceMode::eIMPULSE);
}
/// <summary>
///武器を拾うときに必ず呼ぶ
///とりあえずトリガーをonにする。
/// </summary>
void Weapon::GetWeapon()
{
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
}
