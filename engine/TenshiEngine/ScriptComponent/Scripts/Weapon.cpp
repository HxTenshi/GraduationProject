#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Sandbag.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Initialize(){
	is_hand = false;
	m_weapon_rot = 0.0f;
	is_ground_hit = true;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Start(){
}

//���t���[���Ă΂�܂�
void Weapon::Update(){	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
	ThrowAwayAction();
	m_weapon_rot += Hx::DeltaTime()->GetDeltaTime();
	//PierceSupport(gameObject);
	//Hx::Debug()->Log(std::to_string(gameObject->mTransform->DegreeRotate().x));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Weapon::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Weapon::OnCollideEnter(GameObject target) {
	(void)target;
	if (target->GetLayer() == 3&is_hand){
		//�T���h�o�b�O�ւ̃_���[�W�̏���
		if (auto scr = target->GetScript<Sandbag>()) {
			if (m_Recast > 1.0f&&!is_ground_hit) {
				m_Recast = 0.0f;
				scr->Damage(m_AttackForce);
			}
		}
	}

}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Weapon::OnCollideExit(GameObject target){
	(void)target;
}
/// <summary>
///����ւ̃_���[�W
/// </summary>
void Weapon::Damage(int damage)
{
	m_Endurance -= damage;
}
/// <summary>
///���킪��ꂽ���̔���
/// </summary>
bool Weapon::isBreak()
{
	return (m_Endurance <= 0) ? true : false;
}
/// <summary>
///������̂Ă鏈��
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
///������̂Ă鏈��
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
///������E���Ƃ��ɕK���Ă�
///�Ƃ肠�����g���K�[��on�ɂ���B
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