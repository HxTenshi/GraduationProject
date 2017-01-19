#include"exception.h"

#include <bitset>
#include <sstream>

#pragma comment(lib,"dbghelp.lib")
#include <DbgHelp.h>

#include "SystemLog.h"

int Debug::filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {


	//HMODULE hm;
	//::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPCTSTR>(ep->ExceptionRecord->ExceptionAddress), &hm);
	//MODULEINFO mi;
	//::GetModuleInformation(::GetCurrentProcess(), hm, &mi, sizeof(mi));
	//char fn[MAX_PATH];
	//::GetModuleFileNameExA(::GetCurrentProcess(), hm, fn, MAX_PATH);
	//
	//std::ostringstream oss;
	//oss << "SE " << (seDescription(code)) << " at address 0x" << std::hex << ep->ExceptionRecord->ExceptionAddress << std::dec
	//	<< " inside " << fn << " loaded at base address 0x" << std::hex << mi.lpBaseOfDll << "\n";
	//
	//
	//Hx::Debug()->Log(" Except�G���[ Address[" + oss.str() + "]");

	std::ostringstream oss;

	{
		STACKFRAME sf;
		BOOL bResult;
		PIMAGEHLP_SYMBOL pSym;
		DWORD Disp;

		printf("��O�����B\n");

		/* �V���{�����i�[�p�o�b�t�@�̏����� */
		pSym = (PIMAGEHLP_SYMBOL)GlobalAlloc(GMEM_FIXED, 10000);
		pSym->SizeOfStruct = 10000;
		pSym->MaxNameLength = 10000 - sizeof(IMAGEHLP_SYMBOL);


		IMAGEHLP_LINE line;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		/* �X�^�b�N�t���[���̏����� */
		ZeroMemory(&sf, sizeof(sf));
		sf.AddrPC.Offset = ep->ContextRecord->Eip;
		sf.AddrStack.Offset = ep->ContextRecord->Esp;
		sf.AddrFrame.Offset = ep->ContextRecord->Ebp;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrStack.Mode = AddrModeFlat;
		sf.AddrFrame.Mode = AddrModeFlat;

		/* �V���{���n���h���̏����� */
		SymInitialize(GetCurrentProcess(), NULL, TRUE);

		/* �X�^�b�N�t���[�������ɕ\�����Ă��� */
		for (;;) {
			/* ���̃X�^�b�N�t���[���̎擾 */
			bResult = StackWalk(
				IMAGE_FILE_MACHINE_I386,
				GetCurrentProcess(),
				GetCurrentThread(),
				&sf,
				NULL,
				NULL,
				SymFunctionTableAccess,
				SymGetModuleBase,
				NULL);

			/* ���s�Ȃ�΁A���[�v�𔲂��� */
			if (!bResult || sf.AddrFrame.Offset == 0) break;

			/* �v���O�����J�E���^����֐������擾 */
			bResult = SymGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &Disp, pSym);
			bResult = bResult && SymGetLineFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &Disp, &line);

			/* �擾���ʂ�\�� */
			if (bResult) {
				std::string c = pSym->Name;
				{
					auto i = c.find(":");
					if (i != std::string::npos) {
						c = c.erase(i);
						c += ".cpp";
					}
				}

				oss << c << ":line(" << line.LineNumber << "):" << pSym->Name << "()";
				_SYSTEM_LOG(oss.str());
				oss.clear();
				//printf("0x%08x %s() + 0x%x\n", sf.AddrPC.Offset, pSym->Name, Disp);
				//Hx::Debug()->Log(oss.str());
			}
			else {
				//std::ostringstream oss;
				//oss << " address 0x" << std::hex << sf.AddrPC.Offset << std::dec << " ---";
				//printf("%08x, ---", sf.AddrPC.Offset);
				//Hx::Debug()->Log(oss.str());
			}
		}

		/* �㏈�� */
		SymCleanup(GetCurrentProcess());
		GlobalFree(pSym);

	}


	//auto address = (unsigned long)ep->ExceptionRecord->ExceptionAddress;
	//auto ecode = ep->ExceptionRecord->ExceptionCode;
	//
	//{
	//	std::ostringstream os;
	//	os << std::hex << address;
	//	Hx::Debug()->Log(" Except�G���[ Address[" + os.str() + "]");
	//}
	{
		//std::ostringstream os;
		//os << std::hex << code;
		//Hx::Debug()->Log(" +-- Code[" + os.str() + "]");
	}
	if (code == EXCEPTION_FLT_DIVIDE_BY_ZERO) {
		oss << "[�[�����Z]";
	}
	else if (code == EXCEPTION_INT_DIVIDE_BY_ZERO) {
		oss << "[�[�����Z]";
	}
	else if (code == EXCEPTION_ACCESS_VIOLATION) {
		oss << "[�A�N�Z�X�ᔽ]";
	}

	return EXCEPTION_EXECUTE_HANDLER;
}