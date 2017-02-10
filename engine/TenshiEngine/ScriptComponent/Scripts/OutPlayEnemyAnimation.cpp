#include "OutPlayEnemyAnimation.h"
#include "EnemyRezardMan.h"
# include "h_standard.h"

void OutPlayEnemyAnimation::SetInstanceEnemy(GameObject enemy)
{
	mEnemy = enemy;
}

void OutPlayEnemyAnimation::Update()
{
	if (!mIsStartEvent)return;
	//if (!m_Target)return;

	mTime += Hx::DeltaTime()->GetDeltaTime();

	if (mIsStartEvent && mTime > mWaitTime)
	{
		mIsStartEvent = false;
		//‚±‚±‚©‚¦‚é
		//static_cast<EnemyRezardMan*>(Enemy::GetEnemy(Sender))->MoveFrontStart(mWalkTime);
		if (!m_AnimationEndAction)return;
		auto scr = OutputGimic::GetOutputGimic(m_AnimationEndAction);
		if (!scr)return;
		scr->OnStart(gameObject);
	}
}

bool OutPlayEnemyAnimation::OnStart(GameObject Sender)
{
	Hx::Debug()->Log("ŒÄ‚Î‚ê‚½");
	if (isOneAction)return false;
	isOneAction = true;
	Hx::Debug()->Log("------------------------");
	Hx::Debug()->Log(Sender->Name());
	Hx::Debug()->Log("------------------------");
	static_cast<EnemyRezardMan*>(Enemy::GetEnemy(mEnemy))->MoveFrontStart(mWalkTime);
	//static_cast<EnemyRezardMan*>(Enemy::GetEnemy(Sender))->MoveFrontStart(mWalkTime);
	mIsStartEvent = true;
	return true;
}