#include "GetWeapon.h"
#include "h_standard.h"
#include "WeaponHand.h"
#include "Weapon.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetWeapon::Initialize(){
	mMinWeapon = NULL;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetWeapon::Start(){

}

//���t���[���Ă΂�܂�
void GetWeapon::Update(){
	if (Input::Trigger(MouseCoord::Right)) {
		if (mMinWeapon) {
			if (mWeaponHand) {
				auto particle1 = Hx::Instance(mGetParticle1);
				auto particle2 = Hx::Instance(mGetParticle2);
				if (particle1) {
					particle1->mTransform->WorldPosition(mMinWeapon->mTransform->WorldPosition());
					particle1->mTransform->WorldQuaternion(mMinWeapon->mTransform->WorldQuaternion());
				}
				if (particle2) {
					particle2->mTransform->SetParent(mWeaponHand);
					particle2->mTransform->Position(XMVectorZero());
					particle2->mTransform->Quaternion(XMQuaternionIdentity());
				}
				auto weaponHand = mWeaponHand->GetScript<WeaponHand>();
				weaponHand->SetWeapon(mMinWeapon);
			}
		}
	}

	mMinWeapon = NULL;
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GetWeapon::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetWeapon::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetWeapon::OnCollideEnter(GameObject target){

	if (!target)return;
	if (!target->GetScript<Weapon>())return;

	auto p1 = gameObject->mTransform->WorldPosition();
	if (mMinWeapon) {
		auto p2 = mMinWeapon->mTransform->WorldPosition();
		auto p3 = target->mTransform->WorldPosition();
		auto l1 = XMVector3Length(p1 - p2).x;
		auto l2 = XMVector3Length(p1 - p3).x;
		if (l1 > l2) {
			mMinWeapon = target;
		}
	}else{
		mMinWeapon = target;
	}
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GetWeapon::OnCollideExit(GameObject target){
	(void)target;
}