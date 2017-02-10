
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutPlayEnemyAnimation :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

	void SetInstanceEnemy(GameObject enemy);

private:
	//メンバ変数
	SERIALIZE GameObject m_Target;
	//アニメーション終了時までの時間
	SERIALIZE float mTime;
	SERIALIZE float mWaitTime;
	SERIALIZE float mWalkTime;
	SERIALIZE GameObject m_Output;
	//アニメーション終了時の処理
	SERIALIZE GameObject m_AnimationEndAction;

	GameObject mEnemy;

	bool isOneAction;
	bool mIsStartEvent;
};