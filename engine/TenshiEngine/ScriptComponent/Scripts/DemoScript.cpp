#include "DemoScript.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "DemoWeapon.h"

DemoScript::DemoScript()
{

}

void DemoScript::Initialize()
{

}
void DemoScript::Start()
{
	mContainer = SearchChildren("WeaponContainer");
	mWeaponContainer = mContainer->GetScript<WeaponContainer>();

	Hx::Debug()->Log("�ߌ�͂�������");
	Hx::Debug()->Log("DemoWeapon���擾�o���Ă�����A����𓊂��� Pysicx�����ŁB�@���e�n�_�Ɏ኱���ߍ��ފ����ŁB");

	Hx::Debug()->Log("��������A�n����኱���ߍ��ނȂ�DemoWeapon�̎q�ɐn��ƂȂ��̃I�u�W�F�N�g��ݒ肷��B��������ē����镨��Rota��ݒ肵�Ă����B");

	Hx::Debug()->Log("DemoWeapon�̍��W���擾>�B�����܂Ŏ������ړ�������B");
}
void DemoScript::Update()
{
	GameObject camera = Hx::FindActor("Camera");


	float speed = 1;
	XMVECTOR position = gameObject->mTransform->WorldPosition();
	if (Input::Down(KeyCoord::Key_A)) {
		position += XMVectorSet(-speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
	}
	if (Input::Down(KeyCoord::Key_D)) {
		position += XMVectorSet(speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
	}

	//����̓���(���ʂ�)
	if (Input::Down(KeyCoord::Key_E)) {
		Throw();
	}

	if (Input::Down(KeyCoord::Key_Q)) {
		TargetMove();
	}


	//���v���C���[�̈ړ�
	gameObject->mTransform->WorldPosition(position);
}
void DemoScript::Finish()
{

}
void DemoScript::OnCollideBegin(GameObject target)
{
	Hx::Debug()->Log("������������ : "+target->Name());
	//�����������̂��q�ɂ���
	XMVECTOR up = XMVectorSet(1,2,1,0);
	target->mTransform->WorldPosition(gameObject->mTransform->WorldPosition()+up);
	Hx::Debug()->Log("���͐e�q�֌W�O���Ă���");
	//target->mTransform->SetParent(gameObject);
	mWeaponContainer->AddWeapon(target);
}
void DemoScript::OnCollideEnter(GameObject target)
{

}
void DemoScript::OnCollideExit(GameObject target)
{

}

//����̕����֔��ł���
void DemoScript::TargetMove()
{
	//auto weapon = mWeaponContainer->GetWeapon()->mTransform->WorldPosition();
	//auto myposition = gameObject->mTransform->WorldPosition();
	//float dx = weapon.x - myposition.x;
	//float dz = weapon.z - myposition.z;
	//float rad = std::atan2(dz, dx);
	//float angle = rad * 360 / 3.14f * 2 - 90;
	//gameObject->mTransform->Rotate(XMVectorSet(0,angle,0,0));
	//Hx::Debug()->Log(std::to_string(angle));
	//XMVECTOR power = 100 * XMVector3Normalize(gameObject->mTransform->Forward());
	//Hx::Debug()->Log("x"+std::to_string(power.x));
	//Hx::Debug()->Log("y"+std::to_string(power.y));
	//Hx::Debug()->Log("z"+std::to_string(power.z));
	//gameObject->mTransform->AddForce(power);
}

XMVECTOR DemoScript::GetThrowTarget()
{
	GameObject target = Hx::FindActor("Point");
	return XMVECTOR();
}

void DemoScript::Throw()
{
	//mWeaponContainer->gameObject->GetScript<DemoWeapon>()->SetReleaseVector(gameObject, mWeaponContainer->GetWeapon()->mTransform->Position());
}



//���O���肵�Ďq���擾����
GameObject DemoScript::SearchChildren(std::string name)
{
	GameObject targer;
	for(auto i : gameObject->mTransform->Children())
	{
		if (name == i->Name())targer = i;
	}
	return targer;
}
