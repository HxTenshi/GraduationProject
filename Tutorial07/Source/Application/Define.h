#pragma once
#include <Windows.h>


#define WM_MYWMCOLLBACK		(WM_APP+0x0001)
//WPF�E�B���h�E���瑗���郁�b�Z�[�W���`
enum class MyWindowMessage{
	SelectActor,
	ActorDestroy,
	ChangeParamComponent,
	AddComponent,
	RemoveComponent,
	Count,
};