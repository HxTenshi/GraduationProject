
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputStartGimic :public OutputGimic {
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE
	bool m_StartMatineeMode;

	SERIALIZE
	bool m_StartInstanceObjectMode;

	SERIALIZE
	GameObject m_Output;
};