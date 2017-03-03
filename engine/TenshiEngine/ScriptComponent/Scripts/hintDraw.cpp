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

void hintDraw::Initialize()
{
	m_OnStart = true;
	m_NowTextureNumber = 0;
	m_NowTexture = m_Textures[m_NowTextureNumber];	
	m_Timer = 0;
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
		gameObject->mTransform->WorldPosition(XMVectorSet(BIGPOSX, BIGPOSY, 0, 0));
		gameObject->mTransform->WorldScale(XMVectorSet(BIGSIZEX, BIGSIZEY, 0, 0));
		m_Timer += Hx::DeltaTime()->GetDeltaTime();
		if (m_Timer >= m_WaitTime) {
			auto color = mc->mAlbedo;
			color.w -= m_TimeScale * Hx::DeltaTime()->GetDeltaTime();
			if (color.w <= 0.0f) {
				color.w = 1.0f;
				gameObject->mTransform->WorldPosition(XMVectorSet(MINIPOSX, MINIPOSY, 0, 0));
				gameObject->mTransform->WorldScale(XMVectorSet(MINISIZEX, MINISIZEY, 0, 0));
				m_OnStart = false;
			}
			mc->SetAlbedoColor(color);
		}
	}
	else {
		m_Timer = 0;
		m_FirstFrame = true;
	}
}

bool hintDraw::OnStart(GameObject Sender)
{
	m_OnStart = true;
	m_NowTextureNumber++;
	m_NowTexture = m_Textures[m_NowTextureNumber];
	m_Timer = 0;
	m_FirstFrame = true;
	return false;
}