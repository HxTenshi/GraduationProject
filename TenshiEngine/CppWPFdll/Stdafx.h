// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#include "../Source/Application/Define.h"
#define NULL 0
static class Data{
public:
	static HWND GetHWND(){
		if (mhWnd == NULL){
			mhWnd = FindWindow(L"TenshiEngineDummyWindowClass", NULL);
		}
		return mhWnd;
	}
	static void MyPostMessage(MyWindowMessage wm, void* p = NULL);
	static void _SendMessage(UINT wm, WPARAM p1 = NULL, LPARAM p2 = NULL);

private:
	static HWND mhWnd;
};