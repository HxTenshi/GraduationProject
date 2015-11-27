
#pragma once
#include "main.h"

class DllScriptComponent :public IDllScriptComponent{
public:
	DllScriptComponent();
	void Update()override;

private:
	int timer;
	int timer2;
};
class PlayerScript :public IDllScriptComponent{
public:
	PlayerScript();
	void Update()override;
	void OnCollide(Actor* target) override;
	float mRotateY;

};
class CameraScript :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

private:
	float mRotateY;
	float mLength;
};

class unk : public IDllScriptComponent{
	void Update()override;
};