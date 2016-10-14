#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Initialize(){
	m_Endurance = 1;
	m_frame_pos = gameObject->mTransform->WorldPosition();
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Start(){
	m_frame_pos = gameObject->mTransform->WorldPosition();
}

//���t���[���Ă΂�܂�
void Weapon::Update(){


	XMVECTOR pos = gameObject->mTransform->WorldPosition();
	m_throw_dir.x = (m_frame_pos.x - pos.x)*100.0f;
	m_throw_dir.y = 3.0f;
	m_throw_dir.z = (m_frame_pos.z - pos.z)*100.0f;
	m_frame_pos= gameObject->mTransform->WorldPosition();
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
	gameObject->GetComponent<PhysXComponent>()->AddForce(m_throw_dir, ForceMode::eIMPULSE);
}
/// <summary>
///������E���Ƃ��ɕK���Ă�
///�Ƃ肠�����g���K�[��on�ɂ���B
/// </summary>
void Weapon::GetWeapon()
{
	gameObject->GetComponent<PhysXColliderComponent>()->SetIsTrigger(true);
}
