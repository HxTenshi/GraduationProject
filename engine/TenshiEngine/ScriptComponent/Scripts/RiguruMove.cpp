#include "RiguruMove.h"

#include "h_component.h"
#include "h_standard.h"

void RiguruMove::Initialize()
{
}

void RiguruMove::Start()
{
}

void RiguruMove::Update()
{
	XMVECTOR position = gameObject->mTransform->WorldPosition();
	if(Input::Down(KeyCode::Key_W)) {
		position += XMVectorSet(0, 0, speed, 0) * Hx::DeltaTime()->GetDeltaTime();
		//gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(0, 0, speed, 0) ,ForceMode::eFORCE);
	}
	if (Input::Down(KeyCode::Key_S)) {
		position += XMVectorSet(0, 0, -speed, 0) * Hx::DeltaTime()->GetDeltaTime();
		//gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(0, 0, -speed, 0), ForceMode::eFORCE);
	}
	if (Input::Down(KeyCode::Key_A)) {
		position += XMVectorSet(-speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
		//gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(-speed, 0, 0, 0), ForceMode::eFORCE);
	}
	if (Input::Down(KeyCode::Key_D)) {
		position += XMVectorSet(speed, 0, 0, 0) * Hx::DeltaTime()->GetDeltaTime();
		//gameObject->GetComponent<PhysXComponent>()->AddForce(XMVectorSet(speed, 0, 0, 0) , ForceMode::eFORCE);
	}

	gameObject->mTransform->WorldPosition(position);
}

void RiguruMove::Finish()
{
}

void RiguruMove::OnCollideBegin(GameObject target)
{
}

void RiguruMove::OnCollideEnter(GameObject target)
{
}

void RiguruMove::OnCollideExit(GameObject target)
{
}
