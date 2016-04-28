//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//#define _XM_NO_INTRINSICS_
// /D "_XM_NO_INTRINSICS_" 

#include <d3d11.h>
#include "XNAMath/xnamath.h"

#include "Window/Window.h"
#include "Device/DirectX11Device.h"

#include "MySTL/ptr.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "Graphic/Model/Model.h"
#include "Graphic/Material/Material.h"

#include "ConstantBuffer/ConstantBufferDif.h"

#include "Game/Game.h"

#include "Graphic/Font/Font.h"
//#include "Input\InputManager.h"

class InputManagerRapper{
public:

	void Initialize(HWND hWnd, HINSTANCE hInstance){
		mScreenFocus = false;
		mr = false;
		ml = false;
		mx = 0;
		my = 0;
		wx = 1;
		wy = 1;

		InputManager::InitDirectInput(hWnd, hInstance);
		Window::SetMouseEvents(&mScreenFocus,&ml, &mr, &mx, &my, &wx, &wy);
	}

	void Update(){

		InputManager::SetMouseL(ml);
		InputManager::SetMouseR(mr);
		int x = (int)((float)mx / wx * WindowState::mWidth);
		int y = (int)((float)my / wy * WindowState::mHeight);
		InputManager::SetMouseXY(x, y);
		InputManager::Update(mScreenFocus);
	}

	void Release(){
		InputManager::Release();
	}

private:
	bool mScreenFocus;
	bool mr, ml;
	int mx, my;
	int wx, wy;
};

#include <memory>


#pragma comment(lib,"lib/Debug/sqlite3.lib")
#include "Library\sqlite3.h"
class db{
	db(){

		// DB�t�@�C����
		char *fileName = "DB/SQLiteDB";
		// DB�I�u�W�F�N�g
		sqlite3 *pDB = NULL;
		// DB�̃I�[�v��
		int err = sqlite3_open(fileName, &pDB);
		if (err != SQLITE_OK){
			/* �G���[���� */
		}

		// �e�[�u���̍쐬
		char *errMsg = NULL;

		err = sqlite3_exec(pDB,
			"CREATE TABLE IF NOT EXISTS myTable "
			"(id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"name CHAR(32) NOT NULL)",
			NULL, NULL, &errMsg);

		if (err != SQLITE_OK){
			printf("%s\n", errMsg);
			sqlite3_free(errMsg);
			errMsg = NULL;
			/* TODO: �G���[���� */
		}

		// �X�e�[�g�����g�I�u�W�F�N�g
		sqlite3_stmt *pStmt = NULL;

		// �f�[�^�̒ǉ�
		// �X�e�[�g�����g�̗p��
		err = sqlite3_prepare_v2(pDB,
			"INSERT INTO myTable (name) VALUES (?)",
			128, &pStmt, NULL);

		if (err != SQLITE_OK){
			/* TODO:�G���[���� */
		}
		else{
			int i = 0;
			for (i = 0; i < 5; i++){
				std::string name = "test" + std::to_string(i);

				// ?�̕����ɒl����
				sqlite3_bind_text(pStmt, 1, name.c_str(), name.length(), SQLITE_STATIC);
				while (SQLITE_DONE != sqlite3_step(pStmt)){}
			}
		}

		// �X�e�[�g�����g�̉��
		sqlite3_finalize(pStmt);

		// �f�[�^�̒��o
		// �X�e�[�g�����g�̗p��
		err = sqlite3_prepare_v2(pDB,
			"SELECT * FROM myTable", 64,
			&pStmt, NULL);

		if (err != SQLITE_OK){
			/* TODO:�G���[���� */
		}
		else{
			// �f�[�^�̒��o
			while (SQLITE_ROW == (err = sqlite3_step(pStmt))){
				int id = sqlite3_column_int(pStmt, 0);
				const unsigned char *name = sqlite3_column_text(pStmt, 1);
				printf("id: %d, name: %s\n", id, name);
			}

			if (err != SQLITE_DONE){
				/* TODO: �G���[���� */
			}
		}

		// �X�e�[�g�����g�̉��
		sqlite3_finalize(pStmt);

		// DB�̃N���[�Y
		err = sqlite3_close(pDB);
		if (err != SQLITE_OK){
			/* TODO:�G���[���� */
		}
	}
};



class Application{
public:
	Application()
		:mGame(NULL)
	{
	}

	HRESULT InitDevice(const Window& window)
	{
		HRESULT hr = S_OK;

		hr = Device::Init(window);
		if (FAILED(hr))
			return hr;

		FontManager::Init();

		// Set primitive topology
		//�C���f�b�N�X�̕��с@Z���Ȃ�
		Device::mpImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mInputManagerRapper.Initialize(window.GetMainHWND(), window.mhInstance);

		mGame = new Game();

		return S_OK;
	}

	void Render()
	{

		mInputManagerRapper.Update();

		mGame->Update();

		mGame->Draw();


		ID3D11ShaderResourceView *const pNULL[4] = { NULL, NULL, NULL, NULL };
		Device::mpImmediateContext->PSSetShaderResources(0, 4, pNULL);
		ID3D11SamplerState *const pSNULL[4] = { NULL, NULL, NULL, NULL };
		Device::mpImmediateContext->PSSetSamplers(0, 4, pSNULL);

		Device::mpSwapChain->Present(1, 0);

	}


	void CleanupDevice()
	{

		if (mGame)delete mGame;

		FontManager::Release();

		mInputManagerRapper.Release();

		Device::CleanupDevice();

		Window::Release();
	}
private:

	InputManagerRapper mInputManagerRapper;
	Game* mGame;
};


#include <thread>

/* �X���b�h�v���V�[�W�� */
void ThreadProc(int nThreadNo)
{
	(void)nThreadNo;
	MessageBox(NULL, "thread", "error", MB_OK);
	//Test::NativeFraction f();

	//�|�C���^�ɂ��Ȃ��ƃf�X�g���N�^���Ă΂�Ȃ��H
	//Test::NativeFraction* f = new Test::NativeFraction(1.0f, 4.0f);
	//printf("%.3f", f->GetRatio());
	//delete f;
	ExitThread(0);
}
//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything an4d goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------

inline
bool WhileColl(){
	if (Input::Down(KeyCoord::Key_ESCAPE)){
		throw(0);
	}
	return true;
}

struct throwNull{};
inline
throwNull ForColl(){
	if (Input::Down(KeyCoord::Key_ESCAPE)){
		throw(0);
	}
	return throwNull();
}

template<class T>
T& operator & (T&& t, throwNull& n){
	return t;
}

//�������[�v���
//�C���v�b�g�A�b�v�f�[�g�Ɉˑ����Ȃ��f�o�b�O�C���v�b�g���쐬����K�v����
//#define while(x) while( (x) && WhileColl() )
//#define for(x) for( x & ForColl() )
//
//#undef while
//#undef for


#include "MySTL\Coroutine.h"


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{

	//HANDLE hThread;
	//DWORD dwThreadID;
	//hThread = CreateThread(NULL, 0,
	//	(LPTHREAD_START_ROUTINE)ThreadProc,
	//	(LPVOID)0, 0, &dwThreadID);

	//���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	//�������[���[�N�Ƀu���[�N�|�C���g
	//_CrtSetBreakAlloc(8359);
	

	Window mWindow(hInstance, nCmdShow);
	if (FAILED(mWindow.Init()))
		return 0;

	// Main message loop
	MSG msg = { 0 };
	{
		Application App;
		if (FAILED(App.InitDevice(mWindow)))
		{
			App.CleanupDevice();
			return 0;
		}

		CoroutineSystem cor;

		cor.StartCoroutine([](){

			Window::AddLog("cortest1");

			YIELD_RETURN_NULL;

			Window::AddLog("coltest2");

			YIELD_RETURN_NULL;

			Window::AddLog("coltest3");

			YIELD_RETURN_NULL;


			Window::AddLog("coltest4");

			YIELD_BREAK;

			Window::AddLog("coltest5");

		});
		cor.StartCoroutine([](){

			Window::AddLog("cortest1-1");

			YIELD_RETURN_NULL;

			Window::AddLog("coltest2-1");

			YIELD_RETURN_NULL;

			Window::AddLog("coltest3-1");

			YIELD_RETURN_NULL;


			Window::AddLog("coltest4-1");

			YIELD_BREAK;

			Window::AddLog("coltest5-1");

		});


		while (WM_QUIT != msg.message)
		//for (int i=0; WM_QUIT != msg.message;i++)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				cor.Tick();
				App.Render();
			}
		}
		App.CleanupDevice();
	}



	//CloseHandle(hThread);

	return ( int )msg.wParam;
}