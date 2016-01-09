
#include <list>

#include "ScriptComponent.h"
#include "MySTL/Reflection/Reflection.h"
#include "Window/window.h"

#include "../ScriptComponent/main.h"
typedef IDllScriptComponent* (__cdecl *CreateInstance_)(const char*);
typedef void(__cdecl *DeleteInstance_)(IDllScriptComponent*);

typedef decltype(Reflection::map) (__cdecl *GetReflectionData_)();

#include "Game/Game.h"
SGame gSGame;

//����DLL�������Ă�R���|�[�l���g���Ǘ�
class UseScriptActors{
public:
	UseScriptActors(){
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		ReCompile();
	}
	~UseScriptActors(){
		UnLoad();
	}
	void ReCompile(){
		for (auto& p : mList){
			p->Unload();
		}

		UnLoad();

		char cdir[255];
		GetCurrentDirectory(255, cdir);
		std::string  pass = cdir;
#ifdef _DEBUG
		pass += "/ScriptComponent/createdll_auto_d.bat";
#else
		pass += "/ScriptComponent/createdll_auto.bat";
#endif

		SHELLEXECUTEINFO	sei = { 0 };
		//�\���̂̃T�C�Y
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		//�N�����̃E�C���h�E�n���h��
		sei.hwnd = Window::GetMainHWND();
		//�N����̕\�����
		sei.nShow = SW_SHOWNORMAL;
		//���̃p�����[�^���d�v�ŁA�Z�b�g���Ȃ���SHELLEXECUTEINFO�\���̂�hProcess�����o���Z�b�g����Ȃ��B
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		//�N���v���O����
		sei.lpFile = pass.c_str();
		//�v���Z�X�N��
		if (!ShellExecuteEx(&sei) || (const int)sei.hInstApp <= 32){
			MessageBox(Window::GetMainHWND(), "�t�@�C�����J���܂���ł���", "���s", MB_OK);
			return;
		}
		//�I����҂�
		WaitForSingleObject(sei.hProcess, INFINITE);

		DllLoad();

		for (auto& p : mList){
			p->Load();
		}
	}

	void UnLoad(){
		FreeLibrary(hModule);
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		Reflection::map = NULL;
	}
	void DllLoad(){

		// DLL�̃��[�h
#ifdef _DEBUG
		hModule = LoadLibrary("../ScriptComponent/Debug/ScriptComponent.dll");
#else
		hModule = LoadLibrary("../ScriptComponent/Release/ScriptComponent.dll");
#endif
		if (hModule == NULL)
		{
#ifdef _DEBUG
			hModule = LoadLibrary("ScriptComponent/Debug/ScriptComponent.dll");
#else
			hModule = LoadLibrary("ScriptComponent/Release/ScriptComponent.dll");
#endif
		}


		mCreate = (CreateInstance_)GetProcAddress(hModule, "CreateInstance");
		if (mCreate == NULL)
		{
			UnLoad();
			return;
		}
		mDelete = (DeleteInstance_)GetProcAddress(hModule, "ReleseInstance");
		if (mDelete == NULL)
		{
			UnLoad();
			return;
		}
		mGetReflect = (GetReflectionData_)GetProcAddress(hModule, "GetReflectionData");
		if (mGetReflect == NULL)
		{
			UnLoad();
			return;
		}

		Reflection::map = ((GetReflectionData_)mGetReflect)();
	}
	IDllScriptComponent* Create(const std::string& ClassName){
		if (!mCreate)return NULL;
		//dll�ō쐬�����N���X�C���X�^���X���쐬����
		return ((CreateInstance_)mCreate)(ClassName.c_str());
	}

	void Deleter(IDllScriptComponent* script){
		((DeleteInstance_)mDelete)(script);
	}
	std::list<ScriptComponent*> mList;
	void* mCreate;
	void* mDelete;
	void* mGetReflect;
	HMODULE hModule;

};

UseScriptActors actors;


//static
void ScriptManager::ReCompile(){
	actors.ReCompile();
}

ScriptComponent::ScriptComponent(){
	pDllClass = NULL;
}
ScriptComponent::~ScriptComponent(){
}
void ScriptComponent::Initialize(){
	Load();
	actors.mList.push_back(this);
	if (pDllClass){
		pDllClass->Initialize();
	}
}
void ScriptComponent::Load(){
	if (pDllClass)return;

	//dll�ō쐬�����N���X�C���X�^���X���쐬����
	pDllClass = actors.Create(mClassName);

	if (pDllClass){
		pDllClass->game = &gSGame;
		pDllClass->gameObject = gameObject;
	}
}
void ScriptComponent::Unload(){

	if (pDllClass)
		actors.Deleter(pDllClass);

	pDllClass = NULL;

}
void ScriptComponent::ReCompile(){
	Unload();
	Load();

}
void ScriptComponent::Start(){
	if (pDllClass){
		pDllClass->Start();
	}
}
void ScriptComponent::Update(){

	if (pDllClass){
		pDllClass->Update();
	}
}
void ScriptComponent::Finish(){
	if (pDllClass){
		pDllClass->Finish();
	}
	Unload();
	actors.mList.remove(this);
}

void ScriptComponent::OnCollide(Actor* target){
	if (pDllClass){
		pDllClass->OnCollide(target);
	}
}


void ScriptComponent::CreateInspector(){

	auto data = Window::CreateInspector();
	std::function<void(std::string)> collback = [&](std::string name){
		mClassName = name;
		ReCompile();
	};
	Window::AddInspector(new InspectorStringDataSet("Class", &mClassName, collback), data);

	if (Reflection::map){
		auto infos = Reflection::GetMemberInfos(pDllClass, mClassName);

		for (auto i : infos){
			auto info = Reflection::GetMemberInfo(infos,i);

			if (info.GetTypeName() == "float"){
				float* paramdata = Reflection::Get<float>(info);

				std::function<void(float)> collbackfloat = [=](float f){
					*paramdata = f;
				};

				Window::AddInspector(new InspectorFloatDataSet(info.GetName(), paramdata, collbackfloat), data);
			}
		}

	}

	Window::ViewInspector("Script", this, data);
}