#include "EnemyMinotaurAngerEffect.h"
#include "h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyMinotaurAngerEffect::Initialize(){
	if(obj1)effect.push_back(obj1);
	if(obj2)effect.push_back(obj2);
	if(obj3)effect.push_back(obj3);
	if(obj4)effect.push_back(obj4);
	if(obj5)effect.push_back(obj5);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyMinotaurAngerEffect::Start(){

}

//���t���[���Ă΂�܂�
void EnemyMinotaurAngerEffect::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyMinotaurAngerEffect::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyMinotaurAngerEffect::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyMinotaurAngerEffect::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void EnemyMinotaurAngerEffect::OnCollideExit(GameObject target){
	(void)target;
}

void EnemyMinotaurAngerEffect::Anger()
{
	for (auto i : effect) {
		if (i->GetComponent<ParticleComponent>()) {
			auto cap = i->GetComponent<ParticleComponent>()->mParticleCapacity;
			i->GetComponent<ParticleComponent>()->mParticleParam.Param.x = cap;

		}
	}
}
