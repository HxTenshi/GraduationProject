#include "Weapon.h"
#include "Input/Input.h"
#include "h_component.h"
#include "h_standard.h"
#include "Enemy.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
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

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Start(){
}

//���t���[���Ă΂�܂�
void Weapon::Update(){	
	m_Recast += 1 * Hx::DeltaTime()->GetDeltaTime();
	ThrowAwayAction();
	m_weapon_rot += Hx::DeltaTime()->GetDeltaTime()*10.0f;
	//PierceSupport(gameObject);
	//Hx::Debug()->Log(std::to_string(gameObject->mTransform->DegreeRotate().x));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Weapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Weapon::OnCollideBegin(GameObject target){
	//�������Ƃ������̐���
	if (!target)return;

	if (mIsEnemyThrow) {
		if (mWeaponControl) {
			if (target->GetLayer() == 3)
				if (auto weapon = mWeaponControl->GetScript<WeaponControl>()) {
					Hx::Debug()->Log("�G�ɓ����ē�������");
					Hx::Debug()->Log(target->Name());
					Hx::Debug()->Log(gameObject->Name());
					weapon->HitActor(target, gameObject);
				}
		}
	}
	if (target->GetLayer() == 3 && is_hand) {
		//�T���h�o�b�O�ւ̃_���[�W�̏���
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

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Weapon::OnCollideEnter(GameObject target) {
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
	m_param.Damage(damage);
}
/// <summary>
///���킪��ꂽ���̔���
/// </summary>
bool Weapon::isBreak()
{
	return m_param.isBreak();
}
/// <summary>
///������̂Ă鏈��
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
///������̂Ă鏈��
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
///�U�����̃R�[���o�b�N�֐��ǉ�
/// </summary>
void Weapon::SetHitCollback(const HitCollbackType & collback)
{
	m_HitCollback = collback;
}
/// <summary>
///������E���Ƃ��ɕK���Ă�
///�Ƃ肠�����g���K�[��on�ɂ���B
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
//�\�����ʃo�O������܂�
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
