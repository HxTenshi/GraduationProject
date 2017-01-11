
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputMoveObjectGimic :public OutputGimic {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	bool IsPlay();
	void Play();
	void Stop();

	void SetLoop(bool f);
	bool IsLoop();

	void SetCurrentCameraPointStart(bool flag);
	bool GetCurrentCameraPointStart();

	void SetEndCollback(const std::function<void(void)>& func);
	void SetSpeed(float speed);
	float GetSpeed();

	bool OnStart(GameObject Sender)override;


private:

	void next();

	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_Point_1;
	SERIALIZE GameObject m_Point_2;
	SERIALIZE GameObject m_Point_3;
	SERIALIZE GameObject m_Point_4;
	SERIALIZE GameObject m_Point_5;
	SERIALIZE GameObject m_Point_6;

	SERIALIZE float m_Point_0_EndWaitTime;
	SERIALIZE float m_Point_1_EndWaitTime;
	SERIALIZE float m_Point_2_EndWaitTime;
	SERIALIZE float m_Point_3_EndWaitTime;
	SERIALIZE float m_Point_4_EndWaitTime;
	SERIALIZE float m_Point_5_EndWaitTime;
	SERIALIZE float m_Point_6_EndWaitTime;

	SERIALIZE GameObject m_Point_0_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_1_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_2_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_3_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_4_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_5_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_6_EndTriggerGimmick;


	SERIALIZE
		float m_Speed;

	SERIALIZE
		bool m_Rebirth;
	SERIALIZE
		bool m_Loop;

	SERIALIZE
		GameObject m_Object;
	SERIALIZE
		bool m_CurrentObjectPointStart;
	bool m_IsPlay;
	std::function<void(void)> m_EndCollback;
	float m_Timer;
	float m_TimerMax;
	bool m_WaitMode;
	bool m_RebirthMode;
	int m_CurrentPos;
	XMVECTOR m_position[2];
	XMVECTOR m_quaternion[2];
};