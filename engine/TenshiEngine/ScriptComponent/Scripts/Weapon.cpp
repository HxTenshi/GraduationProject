#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Sandbag.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Initialize(){
	m_Endurance = 1;
	m_AttackForce = 1;
	m_Recast = 1;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Start(){
}

//���t���[���Ă΂�܂�
void Weapon::Update(){
	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Weapon::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
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

	gameObject->GetComponent<PhysXComponent>()->SetGravity(true);

	XMVECTOR wpos = gameObject->mTransform->WorldPosition();
	gameObject->mTransform->SetParent(Hx::GetRootActor());
	gameObject->mTransform->WorldPosition(wpos);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(false);

}
/// <summary>
///������̂Ă鏈��
/// </summary>
void Weapon::ThrowAway(XMVECTOR & throwdir)
{
	ThrowAway();
	gameObject->GetComponent<PhysXComponent>()->AddForce(throwdir, ForceMode::eIMPULSE);
}
/// <summary>
///������E���Ƃ��ɕK���Ă�
///�Ƃ肠�����g���K�[��on�ɂ���B
/// </summary>
void Weapon::GetWeapon()
{
	gameObject->GetComponent<PhysXComponent>()->SetGravity(false);
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
}
