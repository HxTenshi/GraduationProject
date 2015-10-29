#include "Component.h"

#include "../ScriptComponent/main.h"
typedef IDllScriptComponent* (__cdecl *CreateInstance_)();
typedef void(__cdecl *DeleteInstance_)(IDllScriptComponent*);

std::map<std::string, std::function<Component*()>> ComponentFactory::mFactoryComponents;
bool ComponentFactory::mIsInit = false;

//����DLL�������Ă�R���|�[�l���g���Ǘ�
class UseScriptActors{
public:
	UseScriptActors(){
		mEndReloadDLL = false;
	}
	void ReLoad(){
		if (mEndReloadDLL)return;
		mEndReloadDLL = true;
		for (auto& p : mList){
			p->Unload();
		}

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


		for (auto& p : mList){
			p->Load();
		}
	}
	std::list<ScriptComponent*> mList;

	bool mEndReloadDLL;
};
static UseScriptActors actors;

ScriptComponent::ScriptComponent(){
	pDllClass = NULL;
	hModule = NULL;
	mCreate = NULL;
	mDelete = NULL;

	Load();

	actors.mList.push_back(this);

}
ScriptComponent::~ScriptComponent(){
	Unload();
	actors.mList.remove(this);
}

void ScriptComponent::Load(){
	if (pDllClass)return;
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
			FreeLibrary(hModule);
			hModule = NULL;
			return;
		}
		mDelete = (DeleteInstance_)GetProcAddress(hModule, "ReleseInstance");
		if (mDelete == NULL)
		{
			mCreate = NULL;
			FreeLibrary(hModule);
			hModule = NULL;
			return;
		}

		//dll�ō쐬�����N���X�C���X�^���X���쐬����
		pDllClass = ((CreateInstance_)mCreate)();
	}

void ScriptComponent::Unload(){

		if (pDllClass)
			((DeleteInstance_)mDelete)(pDllClass);

		FreeLibrary(hModule);
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		pDllClass = NULL;
	}
void ScriptComponent::ReCompile(){
		Unload();
		Load();

	}
void ScriptComponent::Update(){

		if (pDllClass){
			pDllClass->Update(gameObject);
		}

		if (Input::Trigger(KeyCoord::Key_R)){
			actors.ReLoad();
			return;
			Unload();

			//STARTUPINFO si;// �X�^�[�g�A�b�v���
			//PROCESS_INFORMATION pi;// �v���Z�X���
			//
			////STARTUPINFO �\���̂̓��e���擾 
			//GetStartupInfo(&si);
			//
			//CreateProcess(
			//	NULL,					// ���s�\���W���[���̖��O
			//	"cmd.exe cd ./../ScriptComponent/ cureatedll.bat",			// �R�}���h���C���̕�����
			//	NULL,					// �Z�L�����e�B�L�q�q
			//	NULL,					// �Z�L�����e�B�L�q�q
			//	FALSE,					// �n���h���̌p���I�v�V����
			//	CREATE_NO_WINDOW,				// �쐬�̃t���O 
			//	//CREATE_NEW_PROCESS_GROUP	: �V���ȃv���Z�X
			//	//CREATE_NEW_CONSOLE		: �V�����R���\�[���Ŏ��s
			//	//CREATE_NO_WINDOW			: �R���\�[���E�B���h�E�Ȃ��ŃA�v���P�[�V���������s
			//	NULL,					// �V�������u���b�N
			//	NULL,					// �J�����g�f�B���N�g���̖��O
			//	&si,					// �X�^�[�g�A�b�v���
			//	&pi					// �v���Z�X���
			//	);

			//ShellExecute(Window::GetHWND(), "open", "c:Program Files\\Wireshark\\test.bat", SW_SHOW);
			char cdir[255];
			GetCurrentDirectory(255, cdir);
			std::string  pass = cdir;
#ifdef _DEBUG
			pass += "/ScriptComponent/createdll_auto_d.bat";
#else
			pass += "/ScriptComponent/createdll_auto.bat";
#endif
			//if (ShellExecute(Window::GetHWND(), NULL, pass.c_str(), NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
			//	MessageBox(Window::GetHWND(), "�t�@�C�����J���܂���ł���", "���s", MB_OK);

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

			Load();

		}
		else{
			actors.mEndReloadDLL = false;
		}
	}


#include "Game.h"
void MeshDrawComponent::Update(){

	if (!mModel){
		mModel = gameObject->GetComponent<ModelComponent>();
		if (!mModel)
			mModel = gameObject->GetComponent<TextureModelComponent>();
		if (!mModel)return;
	}
	if (!mModel->mModel)return;

	if (!mMaterial){
		mMaterial = gameObject->GetComponent<MaterialComponent>();
	}
	if (!mMaterial)return;

	Game::AddDrawList(DrawStage::Diffuse, std::function<void()>([&](){
		mModel->SetMatrix();
		Model& model = *mModel->mModel;

		if (mWriteDepth){
			model.Draw(mMaterial);
		}
		else{
			NoWriteDepth(model);
		}
	}));

}