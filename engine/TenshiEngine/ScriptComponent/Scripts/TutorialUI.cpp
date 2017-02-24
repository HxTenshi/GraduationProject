#include "TutorialUI.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "PlayerController.h"
# include "TPSCamera.h"

# include "Game/Component/TextureModelComponent.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TutorialUI::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void TutorialUI::Start(){

}

//���t���[���Ă΂�܂�
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

