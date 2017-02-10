#include "OutputAnimation.h"
#include "../h_standard.h"
#include "../h_component.h"

void OutputAnimation::Update()
{
	if (mIsEnd)return;
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return;
	bool isEnd = mPlayAnimation->IsAnimationEnd(0);
	if (isEnd)
	{
		mIsEnd = true;
		auto scr = OutputGimic::GetOutputGimic(m_AnimationEndAction);
		if (!scr)return;
		scr->OnStart(gameObject);
		Hx::Debug()->Log("アニメーション終了時の処理実行");
	}
}

bool OutputAnimation::OnStart(GameObject Sender)
{
	if (!m_Target)return false;
	mIsEnd = false;
	Hx::Debug()->Log(m_Target->Name());
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return false;
	Hx::Debug()->Log("アニメの設定");

	auto p = mPlayAnimation->GetAnimetionParam(mPlayAnimationID);
	p.mTime = mTime;
	p.mWeight = mWeight;
	p.mTimeScale = mTimeScale;
	p.mLoop = false;
	mPlayAnimation->SetAnimetionParam(mPlayAnimationID, p);

	auto scr = OutputGimic::GetOutputGimic(m_Output);
	if (!scr)return false;
	scr->OnStart(Sender);

	return true;
}
