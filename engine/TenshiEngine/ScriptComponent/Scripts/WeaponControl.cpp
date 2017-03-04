#include "WeaponControl.h"
#include "h_component.h"
#include "h_standard.h"
# include "Game\Component\BoneMirrorComponent.h"
#include "Weapon.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponControl::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponControl::Start(){

}

//���t���[���Ă΂�܂�
void WeaponControl::Update(){

	if (isHit)
	{
		mTime += Hx::DeltaTime()->GetDeltaTime();
		if (mTime > mHitTime) {
			mTime = 0;
			isHit = false;
			Hx::Debug()->Log("��������̈ړ��\�Ȏ��Ԃ𒴉߂��܂����B");
		}
	}

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponControl::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponControl::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponControl::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponControl::OnCollideExit(GameObject target){
	(void)target;
}

void WeaponControl::HitActor(GameObject target,GameObject weapon)
{
	Hx::Debug()->Log("�������Ă���͂�");
	Hx::Debug()->Log(" : "+std::to_string(target->GetLayer()));
	Hx::Debug()->Log(" : " + target->Name());
	if (weapon) {
		if (auto w = weapon->GetScript<Weapon>()) {
			Hx::Debug()->Log("�f���[��");
			w->DeleteEffect(0.9f);
		}
	}
	if (target->GetLayer() == 3){
		Hx::Debug()->Log("����");
		if (SearchEnemyBone(target, weapon, "Head"))return;
		if (SearchEnemyBone(target, weapon, "�Z���^�["))return;
		if (SearchEnemyBone(target, weapon, "master"))return;
		if (SearchEnemyBone(target, weapon, "Spine"))return;
		{//���̑��̓G�i�h�A
			isHit = true;
			if (weapon) {
				if (auto w = weapon->GetScript<Weapon>()) {
					w->SetMirrorTarget(target);
				}
			}
		}
	}
	else if (target->GetLayer() == 12) {
		Hx::Debug()->Log("�̂�");
		SearchEnemyBone(target, weapon, "Spine");
	}


}

//
bool WeaponControl::SearchEnemyBone(GameObject target,GameObject weapon,std::string name)
{
		//�����őΏۂ̓G�̕R�t��
		auto mirrer = weapon->GetComponent<BoneMirrorComponent>();
		if (!mirrer)return false;
		std::list<GameObject> targetObject;
		if (target->GetLayer() == 3){
			targetObject = target->mTransform->Children();
		}
		else if (target->GetLayer() == 12) {
			targetObject = target->mTransform->GetParent()->mTransform->Children();
		}
		GameObject enemyModel = NULL;
		for (auto i : targetObject) {
			if (i->GetLayer() == 10)enemyModel = i;
		}

		mirrer->ChangeTargetBone(enemyModel);
		mirrer->Enable();

		auto vector = mirrer->GetBoneNames();
		int id = 0;
		for (auto& _name : vector)
		{
			if (_name == name)
			{
				break;
			}
			id++;
		}

		if (id < vector.size()) {
			//�Ǐ]����
			mirrer->SetTargetBoneID(id);
			isHit = true;
			if (auto w = weapon->GetScript<Weapon>()) {
				w->SetMirrorTarget(target);
			}
			return true;
		}
		else {
			mirrer->ChangeTargetBone(NULL);
			mirrer->SetTargetBoneID(-1);
			mirrer->Disable();
		}
		return false;

}

//���ɓ���������?
bool WeaponControl::IsHitHead(GameObject target)
{
	//for (GameObject i : target->mTransform->Children())
	//{
	//	if (i->GetLayer() == 12)return true;
	//}
	if (target->GetLayer() == 12)return true;
	return false;
}

bool WeaponControl::IsHit()
{
	return isHit;
}

void WeaponControl::Hit()
{
	isHit = true;
}

void WeaponControl::DeleteHitPoint()
{
	isHit = false;
	mTime = 0;
}

void WeaponControl::HitStage(GameObject target, GameObject weapon,weak_ptr<PhysXComponent> physices)
{
	//Weapon�̎~�܂鏈����ǉ�����
	auto physics = weapon->GetScript<std::weak_ptr<PhysXComponent>>();
	//std::weak_ptr<PhysXComponent> pt = weapon->GetScript<std::weak_ptr<PhysXComponent>>();
	Hx::Debug()->Log(weapon->Name());
	Hx::Debug()->Log(target->Name());
	Hx::Debug()->Log(weapon->GetScript<PhysXColliderComponent>()->ClassName());
	
	if (physices)
	{
		Hx::Debug()->Log("�L�l�}�Ă�����");
		//physics->SetKinematic(true);
	}

}