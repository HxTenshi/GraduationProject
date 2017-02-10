#include "OutputAnimation.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "ObjectGenerator.h"
#include "Enemy.h"
#include "EnemyRezardMan.h"

void OutputAnimation::Update()
{
	if (mIsEnd)return;
	if (!m_Target)return;
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return;
	bool isEnd = mPlayAnimation->IsAnimationEnd(0);
	if (isEnd)
	{
		mIsEnd = true;
		if (m_UseEndAction) {
			auto scr = OutputGimic::GetOutputGimic(m_AnimationEndAction);
			if (!scr)return;
			scr->OnStart(gameObject);
		}
		if (!m_UseGenConb)return;
		Hx::Debug()->Log("アニメーションが終わった");
		auto enemyObj = m_EnemyGen->GetScript<ObjectGenerator>()->GetGeneratorObject();
		if (!enemyObj) {
			Hx::Debug()->Log("enemyが遅れてない");
			return;
		}
		Hx::Debug()->Log("enemyは見えた");
		GameObject child;
		for (auto i : enemyObj->mTransform->Children()) {
			child = i;
		}
		auto rezardScr = static_cast<EnemyRezardMan*>(Enemy::GetEnemy(child));
		if (!rezardScr) {
			Hx::Debug()->Log("scriptが見えない");
			return;
		}
		Hx::Debug()->Log("scriptも見えた");

		rezardScr->MoveFrontStart(m_WalkTime);
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