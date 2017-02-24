#include "TutorialUI.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "PlayerController.h"
# include "TPSCamera.h"

# include "Game/Component/TextureModelComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void TutorialUI::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TutorialUI::Start(){

}

//毎フレーム呼ばれます
void TutorialUI::Update(){

	//auto texture = gameObject->GetComponent<TextureModelComponent>()->SetTexture()


	/*auto character = UniqueObject::GetPlayer()->GetComponent<CharacterControllerComponent>();*/
	auto camera = UniqueObject::GetCamera()->GetScript<TPSCamera>();
	if (camera) {
		GameObject target = camera->GetLookTarget();
		if (target)mBattleUIs->Enable();
		else mBattleUIs->Disable();
	}

}

