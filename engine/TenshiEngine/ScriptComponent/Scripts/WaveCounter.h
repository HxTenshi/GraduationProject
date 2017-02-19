
#pragma once
#include "main.h"
# include "h_component.h"

class WaveCounter :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	void AddCounter();

private:
	//�����o�ϐ�
	SERIALIZE GameObject mText;
	SERIALIZE int mALLWave;
	int mRound;
};