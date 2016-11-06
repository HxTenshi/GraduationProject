#include "Script.h"
//#include "Game\Game.h"
#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/PhysXComponent.h"
#include "Game/Component/TextComponent.h"
#include "Input/Input.h"
#include "MySTL/ptr.h"
#include "Game/Script/IGame.h"
#include "Engine\Debug.h"

#include <functional>
#include <map>

//#define INITGUID
#include <dinput.h>

#include "System\include.h"


class FuctorySetter{
public:
	FuctorySetter(){
#define _ADD(x) mFactory[typeid(x).name()] = [](){ return new x(); }

		//�����ɍ쐬�����N���X��ǉ����܂�

#include "System/factory.h"

#undef _ADD
	}
	std::map<std::string, std::function<IDllScriptComponent*()>> mFactory;
};

FuctorySetter mFact;

IDllScriptComponent* CreateInstance(const char* ClassName){
	std::string name = "class ";
	name += ClassName;
	auto func = mFact.mFactory.find(name);
	if (func == mFact.mFactory.end())return NULL;
	return func->second();
}
void ReleseInstance(IDllScriptComponent* p){
	delete p;
}


#include <bitset>
#include <sstream>

//#include <windows.h>
//#include <eh.h>
//#include <Psapi.h>

//static const char* seDescription(const unsigned int& code)
//{
//	switch (code) {
//	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
//	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
//	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
//	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
//	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
//	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
//	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
//	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
//	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
//	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
//	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
//	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
//	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
//	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
//	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
//	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
//	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
//	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
//	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
//	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
//	default: return "UNKNOWN EXCEPTION";
//	}
//}

#pragma comment(lib,"dbghelp.lib")
#include <DbgHelp.h>
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {


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
				//printf("0x%08x %s() + 0x%x\n", sf.AddrPC.Offset, pSym->Name, Disp);
				//Hx::Debug()->Log(oss.str());
			}
			else {
				//std::ostringstream oss;
				//oss << " address 0x" << std::hex << sf.AddrPC.Offset << std::dec << " ---";
				//printf("%08x, ---", sf.AddrPC.Offset);
				//Hx::Debug()->Log(oss.str());
			}
			break;
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

	Hx::Debug()->Log(oss.str());
	return EXCEPTION_EXECUTE_HANDLER;
}

bool Function0(IDllScriptComponent* com, IDllScriptComponent::Func0 func){
	if (!com){
		return true;
	}
	__try{
		//try{
		(com->*func)();
		//}
		//catch (char* text){
		//	debug->Log(text);
		//}
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())){
		return false;
	}
	return true;
}
GameObject g_Target = NULL;

void _Func1_2(IDllScriptComponent* com, IDllScriptComponent::Func1 func) {

	(com->*func)(g_Target);

}
bool _Func1_1(IDllScriptComponent* com, IDllScriptComponent::Func1 func) {

	__try {
		_Func1_2(com, func);
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		return false;
	}
	return true;
}
bool Function1(IDllScriptComponent* com, IDllScriptComponent::Func1 func, GameObject tar){
	if (!com) {
		return true;
	}
	g_Target = tar;
	auto f = _Func1_1(com, func);
	g_Target = NULL;
	return f;
}