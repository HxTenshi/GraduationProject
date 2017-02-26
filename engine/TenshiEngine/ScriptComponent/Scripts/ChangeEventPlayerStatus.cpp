#include "ChangeEventPlayerStatus.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChangeEventPlayerStatus::Start(){

}

//毎フレーム呼ばれます
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
