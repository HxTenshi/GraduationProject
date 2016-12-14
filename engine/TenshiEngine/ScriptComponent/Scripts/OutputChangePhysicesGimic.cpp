#include "OutputChangePhysicesGimic.h"
#include "../h_standard.h"
#include "../h_component.h"

bool OutputChangePhysicesGimic::OnStart(GameObject Sender)
{
	if (!m_Target)return false;

	if (auto script = m_Target->GetComponent<PhysXComponent>())
	{
		if(mIsEnable)script->SetKinematic(mIsEnable);
		else script->SetKinematic(mIsEnable);
	}

	return true;
}
