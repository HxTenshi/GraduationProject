#include "OutputChangePhysicesGimic.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Matinee.h"

bool OutputChangePhysicesGimic::OnStart(GameObject Sender)
{
	if (!m_Target)return false;

	if (auto script = m_Target->GetComponent<PhysXComponent>())
	{
		if(mIsEnable)script->SetKinematic(mIsEnable);
		else script->SetKinematic(mIsEnable);
		
		if (!m_Output)return false;
		auto gimic = OutputGimic::GetOutputGimic(m_Output);
		if (!gimic)return false;
		gimic->OnStart(gameObject);

	}

	return true;
}
