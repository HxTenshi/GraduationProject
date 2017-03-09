#include "hintDraw.h"
# include "h_standard.h"
# include "Game\Component\MaterialComponent.h"
#include "Graphic\Material\Material.h"
#define BIGSIZEX 1040
#define BIGSIZEY 240
#define BIGPOSX 440
#define BIGPOSY 660

#define MINISIZEX 520
#define MINISIZEY 120
#define MINIPOSX 1100
#define MINIPOSY 1080

static GameObject hintDraw_ = NULL;

float lerp(float x, float y, float s) {
	auto s_ = min(1.0f, max(0.0f, s));
	return x * (1.0f - s_) + y * s_;
}

void hintDraw::Initialize()
{
	if (!hintDraw_)hintDraw_ = this->gameObject;
	else {
		Hx::Debug()->Log("hintDraw : Delete");
		Hx::DestroyObject(this->gameObject);
		return;
	}
	m_OnStart = true;
	m_NowTextureNumber = 0;
	m_NowTexture = m_Textures[m_NowTextureNumber];
	m_Timer = 0;
	m_Lerp = 0;
	m_FirstFrame = true;
}

void hintDraw::Update()
{
	if (m_OnStart) {
		auto mc = gameObject->GetComponent<MaterialComponent>();
		if (!mc)return;
		if (m_FirstFrame) {
			mc->GetMaterialPtr(0)->SetTexture(m_NowTexture);
		}
		m_FirstFrame = false;
		m_Timer += Hx::DeltaTime()->GetDeltaTime();
		if (m_Timer >= m_WaitTime) {
			m_Lerp += m_TimeScale * Hx::DeltaTime()->GetDeltaTime();
			if (m_Lerp >= 1.0f) {
				m_OnStart = false;
			}
		}
		gameObject->mTransform->WorldPosition(XMVectorSet(lerp(BIGPOSX,MINIPOSX, m_Lerp), lerp(BIGPOSY, MINIPOSY, m_Lerp), 9999, 0));
		gameObject->mTransform->WorldScale(XMVectorSet(lerp(BIGSIZEX, MINISIZEX, m_Lerp), lerp(BIGSIZEY, MINISIZEY, m_Lerp), 0, 0));
	}
	else {
		m_Timer = 0;
		m_Lerp = 0;
		m_FirstFrame = true;
	}
}

void hintDraw::Finish()
{
	hintDraw_ = NULL;
}

bool hintDraw::OnStart(GameObject Sender)
{
	auto mc = gameObject->GetComponent<MaterialComponent>();
	if (!mc)return false;

	m_OnStart = true;
	m_NowTextureNumber++;
	m_NowTexture = m_Textures[m_NowTextureNumber];
	m_Timer = 0;
	m_Lerp = 0;
	m_FirstFrame = true;
	return true;
}

bool hintDraw::OnStart_(GameObject Sender)
{
	if (!hintDraw_)return false;
	hintDraw_->GetScript<hintDraw>()->OnStart(Sender);
	return true;
}

void hintDraw::OnFinish()
{
	Finish();
}
