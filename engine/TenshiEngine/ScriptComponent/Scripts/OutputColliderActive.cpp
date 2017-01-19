#include "OutputColliderActive.h"
#include "../h_standard.h"
#include "../h_component.h"

bool OutputColliderActive::OnStart(GameObject Sender)
{
	if (!m_Target)return false;

	Hx::Debug()->Log(m_Target->Name()+" : PhysXColliderComponent�̗L���̊m�F");
	if (!m_Target->GetComponent<PhysXColliderComponent>())return false;

	Hx::Debug()->Log("�����蔻��̗L������");
	if(!mIsEnable)m_Target->GetComponent<PhysXColliderComponent>()->Disable();
	else m_Target->GetComponent<PhysXColliderComponent>()->Enable();
	//m_Target->GetComponent<PhysXColliderComponent>()->SetIsTrigger(mIsEnable);

	Hx::Debug()->Log("Log : "+std::to_string(m_Target->GetComponent<PhysXColliderComponent>()->IsEnabled()));

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return false;
	scr->OnStart(Sender);

	return true;
}