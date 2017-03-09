#include "OutputAnimation.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "ObjectGenerator.h"
#include "Enemy.h"
#include "EnemyRezardMan.h"
# include "EnemyOrc.h"
# include "EnemyMinotaur.h"

void OutputAnimation::Update()
{
	if (mBreak)return;
	if (mIsEnd)return;
	if (!m_Target)return;
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return;
	bool isEnd = mPlayAnimation->IsAnimationEnd(mPlayAnimationID);

	if (isEnd)
	{
		mIsEnd = true;

		//if (mTargetBossGen && mIsBossAction) {
		//	auto idle = m_Target->GetComponent<AnimationComponent>()->GetAnimetionParam(18);
		//	idle.mWeight = 0;
		//	mPlayAnimation->SetAnimetionParam(18, idle);

		//	auto mino = m_Target->mTransform->GetParent();
		//	Hx::Debug()->Log("Call : ミノタウロスの初期化");
		//	AnimeParam ap;
		//	ap = mPlayAnimation->GetAnimetionParam(17);
		//	ap.mWeight = 0;
		//	ap.mLoop = false;
		//	mPlayAnimation->SetAnimetionParam(17, ap);
		//	mino->GetScript<EnemyMinotaur>()->EnemyEmergence(false);
		//	return;
		//}

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
	//ミノタウロス用
	if (mTargetBossGen) {
		Hx::Debug()->Log("1");
		if (mTargetBossGen->GetScript<ObjectGenerator>()->GetGeneratorObject())
		{
			auto target = mTargetBossGen->GetScript<ObjectGenerator>()->GetGeneratorObject();
			m_Target = target->mTransform->Children().front()->mTransform->Children().front();
			Hx::Debug()->Log("Call Mino");

			if (mIsBossAction) {
				auto mino = m_Target->mTransform->Children().front();
				if (!mino)return false;
				Hx::Debug()->Log("Name : "+mino->Name());
				auto minotaur = static_cast<EnemyMinotaur*>(Enemy::GetEnemy(m_Target->mTransform->Children().front()));
				if (!minotaur) {
					return false;
				}
				minotaur->IdleStartAnim();

				return false;
			}

		}
		else {
			Hx::Debug()->Log("まだボスがいない　");
		}
	}

	if (!m_Target)return false;
	if (mBreak)return false;
	
	
	mIsEnd = false;

	Hx::Debug()->Log(m_Target->Name());
	auto mPlayAnimation = m_Target->GetComponent<AnimationComponent>();
	if (!mPlayAnimation)return false;

	Hx::Debug()->Log("2");

	auto p = mPlayAnimation->GetAnimetionParam(mPlayAnimationID);
	p.mTime = mTime;
	p.mWeight = mWeight;
	p.mTimeScale = mTimeScale;
	p.mLoop = false;
	mPlayAnimation->SetAnimetionParam(mPlayAnimationID, p);

	Hx::Debug()->Log("Play ID : "+std::to_string(mPlayAnimation));

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
