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
	IsEnemy(target,weapon);
}


//
void WeaponControl::IsEnemy(GameObject target,GameObject weapon)
{
	//�G�ɓ���������
	if (target->GetLayer() == 3)
	{
		//�����őΏۂ̓G�̕R�t��
		auto mirrer = weapon->GetComponent<BoneMirrorComponent>();
		Hx::Debug()->Log("��������SandBag�̎q��������Layer(10)�����o���Ă��̃I�u�W�F�N�g��Bone�����Ă���");
		auto targetObject = target->mTransform->Children();
		for (auto i : targetObject) {
			if (i->GetLayer() == 10)target = i;
		}
		mirrer->ChangeTargetBone(target);
		mirrer->Enable();

		auto vector = mirrer->GetBoneNames();
		int id = 0;
		for (auto name : vector)
		{
			if (name == "Head")
			{
				break;
			}
			id++;
		}

		//�Ǐ]����
		mirrer->SetTargetBoneID(id);
		
		//�h����
		//weapon->mTransform->SetParent(target);
		//weapon->mTransform->WorldPosition(target->mTransform->WorldPosition()+XMVectorSet(0,2,0,0));
		//����̉�](�h�������)
		//weapon->mTransform->DegreeRotate(XMVectorSet(90,0,0,0));
		//weapon->GetComponent<PhysXComponent>()->SetKinematic(false);
		//weapon->
	}
}
