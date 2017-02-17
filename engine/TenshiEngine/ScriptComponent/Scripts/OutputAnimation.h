
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputAnimation :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

	void SetUpOrcChild(GameObject gen,GameObject point);

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

	SERIALIZE GameObject m_OrcChildOne;
	SERIALIZE GameObject m_OrcChildSecond;
	SERIALIZE bool m_OrcChild;
	SERIALIZE GameObject m_MovePointOne;
	SERIALIZE GameObject m_MovePointSecond;

	SERIALIZE float m_ChildWallTime;
};