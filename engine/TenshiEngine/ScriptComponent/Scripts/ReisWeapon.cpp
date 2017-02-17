#include "ReisWeapon.h"
#include "UniqueObject.h"
#include "reis.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisWeapon::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ReisWeapon::Start(){

}

//���t���[���Ă΂�܂�
void ReisWeapon::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void ReisWeapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisWeapon::OnCollideBegin(GameObject target){
	(void)target;


}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void ReisWeapon::OnCollideEnter(GameObject target){
	if (UniqueObject::IsPlayer(target) && m_Reis) {
		if (auto r = m_Reis->GetScript<reis>()) {
			r->WeaponCallHit();
		}
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void ReisWeapon::OnCollideExit(GameObject target){
	(void)target;
}