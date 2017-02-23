#include "OutputInstaanceObject.h"
# include "OutPlayEnemyAnimation.h"

bool OutputInstaanceObject::OnStart(GameObject Sender)
{
	if (!mInstance.IsLoad())return false;
	GameObject obj = Hx::Instance(mInstance);
	obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
	obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());
	if (!m_Output)return false;
	if (m_Output->GetScript<OutPlayEnemyAnimation>()) {
		m_Output->GetScript<OutPlayEnemyAnimation>()->SetInstanceEnemy(obj);
	}

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return false;
	scr->OnStart(Sender);
	return true;
}