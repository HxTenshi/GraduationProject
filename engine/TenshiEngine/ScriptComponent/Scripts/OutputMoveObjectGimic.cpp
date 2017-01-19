#include "OutputMoveObjectGimic.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void OutputMoveObjectGimic::Initialize(){
	m_WaitMode = false;
	m_IsPlay = false;
	m_RebirthMode = false;
	m_CurrentPos = 0;
	SetEndCollback([]() {});

	m_Timer = 0.0f;
	m_TimerMax = 0.0f;
	m_position[0] = XMVectorZero();
	m_position[1] = XMVectorZero();
	m_quaternion[0] = XMQuaternionIdentity();
	m_quaternion[1] = XMQuaternionIdentity();

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void OutputMoveObjectGimic::Start(){

}

//毎フレーム呼ばれます
void OutputMoveObjectGimic::Update(){
	if (!m_IsPlay)return;
	if (!m_Object)return;

	auto time = Hx::DeltaTime()->GetDeltaTime();
	m_Timer -= time;
	m_Timer = max(m_Timer, 0.0f);
	if (!m_WaitMode) {
		float t = 0.0f;
		if (m_TimerMax != 0.0f) {
			t = m_Timer / m_TimerMax;
		}
		auto quat = XMQuaternionSlerp(m_quaternion[1], m_quaternion[0], t);
		auto pos = XMVectorLerp(m_position[1], m_position[0], t);
		if (m_Timer == 0.0f) {
			m_TimerMax = 0.0f;
			m_WaitMode = true;
			m_CurrentPos += m_RebirthMode ? -1:1;
			next();
			m_CurrentPos = max(min(m_CurrentPos, 6),0);
		}

		//gameObject->mTransform->WorldQuaternion(quat);
		//gameObject->mTransform->WorldPosition(pos);
		m_Object->mTransform->WorldQuaternion(quat);
		m_Object->mTransform->WorldPosition(pos);
	}
	if (m_WaitMode) {
		if (m_Timer == 0.0f) {
			//auto l = XMVector3Length(m_position[1] - m_position[0]).x;
			m_TimerMax = m_Timer = 0.0f;
			if (m_Speed != 0.0f) {
				//m_TimerMax = m_Timer = l / m_Speed;
				m_TimerMax = m_Timer = m_Speed;
			}
			m_WaitMode = false;
			if ((!m_RebirthMode && m_CurrentPos == 6) || (m_CurrentPos == 0 && m_RebirthMode)) {
				if (m_Loop) {
					Stop();
					Play();
				}
				else {
					if (m_Rebirth) {
						m_RebirthMode = !m_RebirthMode;
					}
					Stop();
					m_EndCollback();
				}
			}
		}
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void OutputMoveObjectGimic::Finish(){

}

//コライダーとのヒット時に呼ばれます
void OutputMoveObjectGimic::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void OutputMoveObjectGimic::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void OutputMoveObjectGimic::OnCollideExit(GameObject target){
	(void)target;
}


bool OutputMoveObjectGimic::IsPlay()
{
	return m_IsPlay;
}

void OutputMoveObjectGimic::Play()
{
	if (IsPlay()) {
		if (m_Rebirth) {
			m_CurrentPos += m_RebirthMode ? -1 : 1;
			m_CurrentPos = max(min(m_CurrentPos, 6), 0);
			m_RebirthMode = !m_RebirthMode;
		}
		return;
	}
	m_IsPlay = true;

	if (!m_RebirthMode) {
		m_CurrentPos = 0;
	}
	else {
		m_CurrentPos = 6;
	}
	m_WaitMode = true;
	next();
}

void OutputMoveObjectGimic::Stop()
{
	m_IsPlay = false;
	m_Timer = 0.0f;
}

void OutputMoveObjectGimic::SetLoop(bool f)
{
	m_Loop = f;
}

bool OutputMoveObjectGimic::IsLoop()
{
	return m_Loop;
}

void OutputMoveObjectGimic::SetCurrentCameraPointStart(bool flag)
{
	m_CurrentObjectPointStart = flag;
}

bool OutputMoveObjectGimic::GetCurrentCameraPointStart()
{
	return m_CurrentObjectPointStart;
}

void OutputMoveObjectGimic::SetEndCollback(const std::function<void(void)>& func)
{
	m_EndCollback = func;
}

void OutputMoveObjectGimic::SetSpeed(float speed)
{
	m_Speed = max(speed, 0.0f);
}

float OutputMoveObjectGimic::GetSpeed()
{
	return m_Speed;
}

bool OutputMoveObjectGimic::OnStart(GameObject Sender)
{
	Play();
	return true;

}

void OutputMoveObjectGimic::next()
{

	if (m_CurrentPos == 0) {
		m_Timer = m_Point_0_EndWaitTime;
	}
	if (m_CurrentPos == 1) {
		m_Timer = m_Point_1_EndWaitTime;
	}
	if (m_CurrentPos == 2) {
		m_Timer = m_Point_2_EndWaitTime;
	}
	if (m_CurrentPos == 3) {
		m_Timer = m_Point_3_EndWaitTime;
	}
	if (m_CurrentPos == 4) {
		m_Timer = m_Point_4_EndWaitTime;
	}
	if (m_CurrentPos == 5) {
		m_Timer = m_Point_5_EndWaitTime;
	}
	if (m_CurrentPos == 6) {
		m_Timer = m_Point_6_EndWaitTime;
	}

	if (!m_RebirthMode) {

		if (m_CurrentPos == 0) {
			if (!m_Point_1 || !m_Object || !m_CurrentObjectPointStart) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Object->mTransform->WorldQuaternion();
				m_position[0] = m_Object->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_1->mTransform->WorldQuaternion();
				m_position[1] = m_Point_1->mTransform->WorldPosition();

			}

			//m_Point_0_EndTriggerGimmick;
		}

		if (m_CurrentPos == 1) {
			if (!m_Point_1 || !m_Point_2) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_1->mTransform->WorldQuaternion();
				m_position[0] = m_Point_1->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_2->mTransform->WorldQuaternion();
				m_position[1] = m_Point_2->mTransform->WorldPosition();
			}
		}

		if (m_CurrentPos == 2) {
			if (!m_Point_2 || !m_Point_3) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_2->mTransform->WorldQuaternion();
				m_position[0] = m_Point_2->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_3->mTransform->WorldQuaternion();
				m_position[1] = m_Point_3->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 3) {
			if (!m_Point_3 || !m_Point_4) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_3->mTransform->WorldQuaternion();
				m_position[0] = m_Point_3->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_4->mTransform->WorldQuaternion();
				m_position[1] = m_Point_4->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 4) {
			if (!m_Point_4 || !m_Point_5) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_4->mTransform->WorldQuaternion();
				m_position[0] = m_Point_4->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_5->mTransform->WorldQuaternion();
				m_position[1] = m_Point_5->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 5) {
			if (!m_Point_5 || !m_Point_6) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_5->mTransform->WorldQuaternion();
				m_position[0] = m_Point_5->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_6->mTransform->WorldQuaternion();
				m_position[1] = m_Point_6->mTransform->WorldPosition();
			}
		}
	}
	else {

		if (m_CurrentPos == 6) {
			m_CurrentPos += m_RebirthMode ? -1 : 1;
		}

		if (m_CurrentPos == 5) {
			if (!m_Point_5 || !m_Point_6) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_6->mTransform->WorldQuaternion();
				m_position[0] = m_Point_6->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_5->mTransform->WorldQuaternion();
				m_position[1] = m_Point_5->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 4) {
			if (!m_Point_4 || !m_Point_5) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_5->mTransform->WorldQuaternion();
				m_position[0] = m_Point_5->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_4->mTransform->WorldQuaternion();
				m_position[1] = m_Point_4->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 3) {
			if (!m_Point_3 || !m_Point_4) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_4->mTransform->WorldQuaternion();
				m_position[0] = m_Point_4->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_3->mTransform->WorldQuaternion();
				m_position[1] = m_Point_3->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 2) {
			if (!m_Point_2 || !m_Point_3) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_3->mTransform->WorldQuaternion();
				m_position[0] = m_Point_3->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_2->mTransform->WorldQuaternion();
				m_position[1] = m_Point_2->mTransform->WorldPosition();
			}
		}
		if (m_CurrentPos == 1) {
			if (!m_Point_1 || !m_Point_2) {
				m_CurrentPos += m_RebirthMode ? -1 : 1;
			}
			else {
				m_quaternion[0] = m_Point_2->mTransform->WorldQuaternion();
				m_position[0] = m_Point_2->mTransform->WorldPosition();

				m_quaternion[1] = m_Point_1->mTransform->WorldQuaternion();
				m_position[1] = m_Point_1->mTransform->WorldPosition();
			}
		}
	}

}
