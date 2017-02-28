#include "Matinee.h"

#include "h_standard.h"
#include "h_component.h"

#include "OutputGimic.h"

#include "UniqueObject.h"

Matinee* g_NowPlayingObject = NULL;

Matinee::Matinee()
{
}

//生成時に呼ばれます（エディター中も呼ばれます）
void Matinee::Initialize(){
	m_DeltaTimeScaleBack = 1.0f;
	m_WaitMode = false;
	m_IsPlay = false;
	m_IsPlayReady = false;
	m_CurrentPos = 0;
	SetEndCollback([]() {});

	m_Timer = 0.0f;
	m_TimerMax = 0.0f;
	m_position[0] =XMVectorZero();
	m_position[1] = XMVectorZero();
	m_quaternion[0] = XMQuaternionIdentity();
	m_quaternion[1] = XMQuaternionIdentity();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Matinee::Start(){
	if (!m_Camera) {
		m_Camera = UniqueObject::GetCamera();
	}
}

//毎フレーム呼ばれます
void Matinee::Update(){

	if (m_IsPlayReady) {
		Play();
	}

	if (!m_IsPlay)return;
	if (!m_Camera)return;

	auto time = Hx::DeltaTime()->GetNoScaleDeltaTime();
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
			m_CurrentPos++;
			next();
			m_CurrentPos = min(m_CurrentPos, 6);
		}

		//gameObject->mTransform->WorldQuaternion(quat);
		//gameObject->mTransform->WorldPosition(pos);
		m_Camera->mTransform->WorldQuaternion(quat);
		m_Camera->mTransform->WorldPosition(pos);
	}
	if (m_WaitMode) {
		if (m_Timer == 0.0f) {
			auto l = XMVector3Length(m_position[1] - m_position[0]).x;
			m_TimerMax = m_Timer = 0.0f;
			if (m_Speed != 0.0f) {
				m_TimerMax = m_Timer = l/ m_Speed;
			}
			m_WaitMode = false;
			if (m_CurrentPos == 6) {
				Stop();
				m_EndCollback();
			}
		}
	}


}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Matinee::Finish(){
	Stop();
}

//コライダーとのヒット時に呼ばれます
void Matinee::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Matinee::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Matinee::OnCollideExit(GameObject target){
	(void)target;
}

bool Matinee::IsPlay()
{
	return m_IsPlay;
}

void Matinee::Play()
{
	if (m_IsPlay)return;
	if (!m_Camera)return;

	if (g_NowPlayingObject) {
		m_IsPlayReady = true;
		return;
	}
	if (Hx::DeltaTime()->GetDeltaTime()==0.0f)return;

	m_IsPlayReady = false;
	g_NowPlayingObject = this;

	auto cam = m_Camera->GetComponent<ScriptComponent>();
	if (cam) {
		cam->Disable();
	}

	m_DeltaTimeScaleBack = Hx::DeltaTime()->GetTimeScale();
	Hx::DeltaTime()->SetTimeScale(m_DeltaTimeScale);
	m_IsPlay = true;

	m_CurrentPos = 0;
	m_WaitMode = true;
	next();
}

void Matinee::Stop()
{
	m_IsPlayReady = false;
	if (!m_IsPlay)return;
	if (!m_Camera)return;
	auto cam = m_Camera->GetComponent<ScriptComponent>();
	if (cam) {
		cam->Enable();
	}
	m_IsPlay = false;
	m_Timer = 0.0f;

	Hx::DeltaTime()->SetTimeScale(m_DeltaTimeScaleBack);

	if (g_NowPlayingObject == this) {
		g_NowPlayingObject = NULL;
	}
}

void Matinee::SetCurrentCameraPointStart(bool flag)
{
	m_CurrentCameraPointStart = flag;
}

bool Matinee::GetCurrentCameraPointStart()
{
	return m_CurrentCameraPointStart;
}

void Matinee::SetEndCollback(const std::function<void(void)>& func)
{
	m_EndCollback = func;
}

void Matinee::SetSpeed(float speed)
{
	m_Speed = max(speed, 0.0f);
}

float Matinee::GetSpeed()
{
	return m_Speed;
}

void Matinee::SetDeltaTimeScale(float scale)
{
	m_DeltaTimeScale = max(scale, 0.0f);
}

float Matinee::GetDeltaTimeScale()
{
	return m_DeltaTimeScale;
}

bool Matinee::GlobalNowPlaying()
{
	return (bool)g_NowPlayingObject;
}

void Matinee::next()
{

	if (m_CurrentPos == 0) {
		m_Timer = m_Point_0_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_0_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 1) {
		m_Timer = m_Point_1_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_1_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 2) {
		m_Timer = m_Point_2_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_2_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 3) {
		m_Timer = m_Point_3_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_3_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 4) {
		m_Timer = m_Point_4_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_4_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 5) {
		m_Timer = m_Point_5_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_5_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}
	if (m_CurrentPos == 6) {
		m_Timer = m_Point_6_EndWaitTime;
		if (auto gimick = OutputGimic::GetOutputGimic(m_Point_6_EndTriggerGimmick)) {
			gimick->OnStart(gameObject);
		}
	}

	if (m_CurrentPos == 0) {
		if (!m_Point_1 && m_Camera) {
			m_CurrentPos++;
		}
		else {
			if (m_CurrentCameraPointStart) {
				m_quaternion[0] = m_Camera->mTransform->WorldQuaternion();
				m_position[0] = m_Camera->mTransform->WorldPosition();
			}
			else {
				m_quaternion[0] = m_Point_1->mTransform->WorldQuaternion();
				m_position[0] = m_Point_1->mTransform->WorldPosition();
			}

			m_quaternion[1] = m_Point_1->mTransform->WorldQuaternion();
			m_position[1] = m_Point_1->mTransform->WorldPosition();

		}

		//m_Point_0_EndTriggerGimmick;
	}

	if (m_CurrentPos == 1) {
		if (!m_Point_1 || !m_Point_2) {
			m_CurrentPos++;
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
			m_CurrentPos++;
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
			m_CurrentPos++;
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
			m_CurrentPos++;
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
			m_CurrentPos++;
		}
		else {
			m_quaternion[0] = m_Point_5->mTransform->WorldQuaternion();
			m_position[0] = m_Point_5->mTransform->WorldPosition();

			m_quaternion[1] = m_Point_6->mTransform->WorldQuaternion();
			m_position[1] = m_Point_6->mTransform->WorldPosition();
		}
	}

}

