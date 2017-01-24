
#pragma once
#include "main.h"
#include "OutputGimic.h"

class ChangePlayerState :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:

	SERIALIZE bool m_IsLock;
};