
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputAnimation :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_Target;
	//óLå¯âªñ≥å¯Ç©
	SERIALIZE int mPlayAnimationID;
	SERIALIZE float mWeight;
	SERIALIZE float mTime;
	SERIALIZE float mTimeScale;
	SERIALIZE GameObject m_Output;

	SERIALIZE GameObject m_AnimationEndAction;

	SERIALIZE GameObject m_EnemyGen;
	bool mIsEnd;

	SERIALIZE float m_WalkTime;

	SERIALIZE bool m_UseGenConb;
	SERIALIZE bool m_UseEndAction;
};