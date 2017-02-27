
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutputTextUI :public IDllScriptComponent {
public:
	void Start()override;
	void Update()override;

private:
	SERIALIZE GameObject mTextUI;
	SERIALIZE float mWait;
	SERIALIZE GameObject mEndAction;
	SERIALIZE float mDeleteTime;

	SERIALIZE std::string mPutText;

	SERIALIZE float mTime;
	SERIALIZE bool mIsEnd;

	SERIALIZE GameObject mPlaySound;
	bool mIsPlayEnd;
};