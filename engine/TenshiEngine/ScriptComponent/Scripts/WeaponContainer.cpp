#include "WeaponContainer.h"
#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponContainer::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WeaponContainer::Start(){
	mWeapn = mGetWeapon->GetScript<GetWeapon>();
}

//���t���[���Ă΂�܂�
void WeaponContainer::Update(){
	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WeaponContainer::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponContainer::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void WeaponContainer::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void WeaponContainer::OnCollideExit(GameObject target){
	(void)target;
}

//�擾�ςݕ���̎擾
//GameObject WeaponContainer::GetWeapon()
//{
//	return mWeapons.front();
//}
//����̎擾
void WeaponContainer::AddWeapon(GameObject target)
{
	mWeapons.push_back(target);
}



void WeaponContainer::Release(GameObject weapon)
{
	mWeapons.remove(weapon);
}

//�q�̕�����擾����B
void WeaponContainer::GetChildren()
{
	mWeapons = gameObject->mTransform->Children();
}
