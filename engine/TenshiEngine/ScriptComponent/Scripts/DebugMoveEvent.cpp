#include "DebugMoveEvent.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
//–ˆƒtƒŒ[ƒ€ŒÄ‚Î‚ê‚Ü‚·
void DebugMoveEvent::Update(){
	MoveToPont(mPoint_0,KeyCode::Key_0);
	MoveToPont(mPoint_1, KeyCode::Key_1);
	MoveToPont(mPoint_2, KeyCode::Key_2);
	MoveToPont(mPoint_3, KeyCode::Key_3);
	MoveToPont(mPoint_4, KeyCode::Key_4);



}

bool DebugMoveEvent::OnStart(GameObject Sender)
{
	return false;
}

void DebugMoveEvent::MoveToPont(GameObject target, KeyCode key)
{
	if (target) {
		if (Input::Down(key)) {
			auto character = UniqueObject::GetPlayer()->GetComponent<CharacterControllerComponent>();
			if (character) {
				character->Teleport(target->mTransform->WorldPosition());
			}
		}
	}
}
