#include "OutputAnimation.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "ObjectGenerator.h"
#include "Enemy.h"
#include "EnemyRezardMan.h"
# include "EnemyOrc.h"

void OutputAnimation::Update()
{
	if (mBreak)return;
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
		if (!m_EnemyGen)return;
		auto enemyObj = m_EnemyGen->GetScript<ObjectGenerator>()->GetGeneratorObject();
		if (!enemyObj) {
			return;
		}
		GameObject child;
		for (auto i : enemyObj->mTransform->Children()) {
			child = i;
		}
		auto rezardScr = static_cast<EnemyOrc*>(Enemy::GetEnemy(child));
		if (!rezardScr) {
			return;
		}

		if (m_OrcChild) {
			SetUpOrcChild(m_OrcChildOne,m_MovePointOne);
			SetUpOrcChild(m_OrcChildSecond, m_MovePointSecond);
		}

		rezardScr->MoveFrontStart(m_WalkTime);
	}
}

bool OutputAnimation::OnStart(GameObject Sender)
{
	if (!m_Target)return false;
	if (mBreak)return false;
	
	//ミノタウロス用
	if (mTargetBossGen) {
		if (mTargetBossGen->mTransform->Children().front()->mTransform->Children().front()->GetScript<ObjectGenerator>()->GetGeneratorObject())
		{
			m_Target = mTargetBossGen->mTransform->Children().front()->mTransform->Children().front()->GetScript<ObjectGenerator>()->GetGeneratorObject();
		}
	}
	
	
	mIsEnd = false;
	Hx::Debug()->Log(m_Target->Name());
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return false;

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

void OutputAnimation::SetUpOrcChild(GameObject gen, GameObject point)
{
	//if (!gen)return;
	auto enemyObj = gen->GetScript<ObjectGenerator>()->GetGeneratorObject();
	if (!enemyObj) {
		return;
	}
	GameObject child;
	for (auto i : enemyObj->mTransform->Children()) {
		child = i;
	}
	auto orcSrc = static_cast<EnemyOrc*>(Enemy::GetEnemy(child));
	orcSrc->SetMovePoint(point);
	orcSrc->MoveFrontStart(m_ChildWallTime);
}
