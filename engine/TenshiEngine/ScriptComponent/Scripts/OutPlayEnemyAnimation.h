
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutPlayEnemyAnimation :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

	void SetInstanceEnemy(GameObject enemy);

private:
	//�����o�ϐ�
	SERIALIZE GameObject m_Target;
	//�A�j���[�V�����I�����܂ł̎���
	SERIALIZE float mTime;
	SERIALIZE float mWaitTime;
	SERIALIZE float mWalkTime;
	SERIALIZE GameObject m_Output;
	//�A�j���[�V�����I�����̏���
	SERIALIZE GameObject m_AnimationEndAction;

	GameObject mEnemy;

	bool isOneAction;
	bool mIsStartEvent;
};