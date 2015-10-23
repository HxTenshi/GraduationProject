
#include "Window.h"
#include "Input/Input.h"
#include "Graphic/Font/Font.h"


UINT WindowState::mWidth = 1200;
UINT WindowState::mHeight = 800;

UINT WindowState::mTreeViewWidth = 200;
UINT WindowState::mTreeViewHeight = 0;


HMODULE Window::mhModuleWnd = NULL;
HWND Window::mhWnd = NULL;
GameScreenWindow Window::mGameScreenWindow;
TreeViewWindow Window::mTreeViewWindow;
InspectorWindow Window::mInspectorWindow;

Test::NativeFraction Window::mMainWindow_WPF;
HWND Window::mGameScreenHWND = NULL;

std::vector<const std::function<void(void*)>> Window::mWPFCollBacks;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//BitBlt(hdc, 0, 0, WindowState::mWidth, WindowState::mHeight, hdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		// �}�E�X���ړ������Ƃ��̏���
		InputManager::SetMouseP(lParam);
		break;

	case WM_LBUTTONDOWN:
		// �}�E�X���{�^�����N���b�N���ꂽ�Ƃ��̏���
		InputManager::SetMouseL(true);
		break;
	case WM_LBUTTONUP:
		// �}�E�X���{�^�����N���b�N�������ꂽ�Ƃ��̏���

		//�E�B���h�E�O�ŗ����ƃ_��
		InputManager::SetMouseL(false);
		break;

	case WM_RBUTTONDOWN:
		// �}�E�X�E�{�^�����N���b�N���ꂽ�Ƃ��̏���
		InputManager::SetMouseR(true);
		break;
	case WM_RBUTTONUP:
		// �}�E�X�E�{�^�����N���b�N�������ꂽ�Ƃ��̏���
		InputManager::SetMouseR(false);
		break;

	case WM_DROPFILES:{
		HDROP hDrop = (HDROP)wParam;
		char szFileName[256];
		//D&D�����t�@�C���̐�
		int FileNum = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
		for (int i = 0; i < (int)FileNum; i++) {
			DragQueryFile(hDrop, i, szFileName, sizeof(szFileName));
			if (ShellExecute(hWnd, NULL, szFileName, NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
				MessageBox(hWnd, "�t�@�C�����J���܂���ł���", "���s", MB_OK);
		}
		DragFinish(hDrop);
		break;
	}
	case WM_COMMAND:{
		//�����Ō��X�̃v���V�[�W�A�̃A�h���X���擾����B
		HWND childHWND = GetDlgItem(hWnd, (int)LOWORD(wParam));
		if (!childHWND)return DefWindowProc(hWnd, message, wParam, lParam);
		ChildWindow* thisPtr = (ChildWindow*)GetWindowLong(childHWND, GWL_USERDATA);
		if (!thisPtr){//�擾�ł��Ȃ������ꍇ�i�쐬�Ɏ��s���Ă�)
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else{//�擾�ł����ꍇ
			thisPtr->MyWndProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case (UINT_PTR)IDC::ForlderTree:{
			//�����Ō��X�̃v���V�[�W�A�̃A�h���X���擾����B
			HWND childHWND = GetDlgItem(hWnd, (int)IDC::ForlderTree);
			ChildWindow* thisPtr = (ChildWindow*)GetWindowLong(childHWND, GWL_USERDATA);
			if (!thisPtr){//�擾�ł��Ȃ������ꍇ�i�쐬�Ɏ��s���Ă�)
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			else{//�擾�ł����ꍇ
				thisPtr->MyWndProc(hWnd, message, wParam, lParam);
				break;
			}
		}
			break;
		default:{
			//�����Ō��X�̃v���V�[�W�A�̃A�h���X���擾����B
			HWND childHWND = GetDlgItem(hWnd, (int)((LPNMHDR)lParam)->idFrom);
			if (!childHWND)return DefWindowProc(hWnd, message, wParam, lParam);
			ChildWindow* thisPtr = (ChildWindow*)GetWindowLong(childHWND, GWL_USERDATA);
			if (!thisPtr){//�擾�ł��Ȃ������ꍇ�i�쐬�Ɏ��s���Ă�)
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			else{//�擾�ł����ꍇ
				thisPtr->MyWndProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		}
		break;

	case WM_CREATE:
		break;

	case WM_MYWMCOLLBACK:{
		const auto& func = Window::mWPFCollBacks[(int)wParam];
		if (func)func((void*)lParam);
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}