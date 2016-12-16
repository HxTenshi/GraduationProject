#include "WeaponControl.h"
#include "h_component.h"
#include "h_standard.h"
# include "Game\Component\BoneMirrorComponent.h"

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
	if (target->GetLayer() == 3){
		Hx::Debug()->Log("����");
		SearchEnemyBone(target, weapon, "Head");
	}
	else if (target->GetLayer() == 12) {
		Hx::Debug()->Log("�̂�");
		SearchEnemyBone(target, weapon, "Spine");
	}

}

//
void WeaponControl::SearchEnemyBone(GameObject target,GameObject weapon,std::string name)
{
		//�����őΏۂ̓G�̕R�t��
		auto mirrer = weapon->GetComponent<BoneMirrorComponent>();
		std::list<GameObject> targetObject;
		if (target->GetLayer() == 3){
			targetObject = target->mTransform->Children();
		}
		else if (target->GetLayer() == 12) {
			targetObject = target->mTransform->GetParent()->mTransform->Children();
		}

		for (auto i : targetObject) {
			if (i->GetLayer() == 10)target = i;
		}

		mirrer->ChangeTargetBone(target);
		mirrer->Enable();

		auto vector = mirrer->GetBoneNames();
		int id = 0;
		for (auto name : vector)
		{
			if (name == name)
			{
				break;
			}
			id++;
		}

		//�Ǐ]����
		mirrer->SetTargetBoneID(id);
		isHit = true;
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

void WeaponControl::DeleteHitPoint()
{
	isHit = false;
	mTime = 0;
}

void WeaponControl::HitStage(GameObject target, GameObject weapon)
{
	//Weapon�̎~�܂鏈����ǉ�����
	auto physics = weapon->GetScript<PhysXComponent>();
	//Hx::Debug()->Log(weapon->Name());
	//Hx::Debug()->Log(std::to_string(physics->gameObject));
	if (physics)
	{
		Hx::Debug()->Log("�L�l�}�Ă�����");
		physics->SetKinematic(true);
	}

}