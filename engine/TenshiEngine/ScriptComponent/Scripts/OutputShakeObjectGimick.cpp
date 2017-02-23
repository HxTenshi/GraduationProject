#include "OutputShakeObjectGimick.h"
#include "UniqueObject.h"
#include "h_standard.h"
void OutputShakeObjectGimick::Initialize()
{
	
	m_Vect = XMVectorZero();
	m_Timer = 0.0f;
}

void OutputShakeObjectGimick::Update()
{

	if (!m_ShakeObject) {
		m_Vect = XMVectorZero();
		m_Timer = 0.0f;
		return;
	}
	if (m_Timer > 0.0f) {
		if (m_NoScaleDeltaTimeMode) {
			m_Timer -= Hx::DeltaTime()->GetNoScaleDeltaTime();
		}
		else {
			m_Timer -= Hx::DeltaTime()->GetDeltaTime();
		}


		m_Timer = max(m_Timer, 0.0f);

		if (m_Timer == 0.0f) {
			auto pos = m_ShakeObject->mTransform->WorldPosition();
			pos -= m_Vect;
			m_ShakeObject->mTransform->WorldPosition(pos);
			m_Vect = XMVectorZero();
		}
		else {

			auto pow = rand() / (float)RAND_MAX;
			auto x = 1.0f - rand() / (float)RAND_MAX * 2.0f;
			auto y = 1.0f - rand() / (float)RAND_MAX * 2.0f;
			auto z = 1.0f - rand() / (float)RAND_MAX * 2.0f;
			auto v = XMVectorSet(x, y, z, 1.0f);
			if (XMVector3Length(v).x != 0.0f) {
				v = XMVector3Normalize(v) * pow * m_ShakePower;
			}

			auto pos = m_ShakeObject->mTransform->WorldPosition();
			pos += v;
			pos -= m_Vect;
			m_ShakeObject->mTransform->WorldPosition(pos);
			m_Vect = v;
		}
		
	}
}

bool OutputShakeObjectGimick::OnStart(GameObject Sender)
{

	if (m_Camera) {
		m_ShakeObject = UniqueObject::GetCamera();
	}
	if (m_ShakeObject) {
		m_Timer = m_Time;
	}

	return true;
}
