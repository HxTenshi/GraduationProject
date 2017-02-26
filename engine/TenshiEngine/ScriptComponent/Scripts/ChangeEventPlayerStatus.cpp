#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChangeEventPlayerStatus::Start(){

}

//���t���[���Ă΂�܂�
void ChangeEventPlayerStatus::Update(){
	OnSetSP();
}

void ChangeEventPlayerStatus::OnSetSP()
{
	auto player = UniqueObject::GetPlayer()->GetScript<PlayerController>();
	if (player) {
		player->SetSpecial(mSP);
	}
}
