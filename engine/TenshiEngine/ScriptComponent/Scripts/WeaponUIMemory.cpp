#include "WeaponUIMemory.h"
#include "h_standard.h"
#include "h_component.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponUIMemory::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponUIMemory::Start(){
	auto mem = gameObject->mTransform->Children();
	for (auto i : mem) {
		int num = std::stoi(i->Name());
		if (i->GetComponent<MaterialComponent>()) {
			i->GetComponent<MaterialComponent>()->SetAlbedoColor(GetMemColor(num));
			m_GageMem[num - 1] = i;
		}
	}
	FillAlpha();
}

//���t���[���Ă΂�܂�
void WeaponUIMemory::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponUIMemory::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponUIMemory::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponUIMemory::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponUIMemory::OnCollideExit(GameObject target){
	(void)target;
}

XMFLOAT4 WeaponUIMemory::GetMemColor(int n)
{
	if (n > 5) {
		return XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (n > 2) {
		return XMFLOAT4(1.0f, 0.6f, 0.0f, 1.0f);
	}
	else if (n >= 0) {
		return XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	}

	return XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void WeaponUIMemory::FillAlpha()
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_GageMem[i]) {
			m_GageMem[i]->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}

void WeaponUIMemory::WeaponAttackParamMemSet(int atk)
{
	FillAlpha();
	int num = (atk > 10) ? 10 : atk;
	for (int i = 0; i < num; ++i) {
		if (m_GageMem[i]) {
			m_GageMem[i]->GetComponent<MaterialComponent>()->SetAlbedoColor(GetMemColor(i));
		}
	}
}
