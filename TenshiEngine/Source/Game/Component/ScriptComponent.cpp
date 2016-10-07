
#include <list>
#include <iostream>
#include <algorithm>
#include <filesystem> // std::tr2::sys::path etc.

#include "Application/BuildVersion.h"

#include "ScriptComponent.h"
#include "MySTL/Reflection/Reflection.h"
#include "Window/window.h"
#include "Engine/Inspector.h"
#include "Engine/AssetLoad.h"

#include "../ScriptComponent/main.h"
typedef IDllScriptComponent* (__cdecl *CreateInstance_)(const char*);
typedef void(__cdecl *DeleteInstance_)(IDllScriptComponent*);

typedef decltype(Reflection::map) (__cdecl *GetReflectionData_)();

typedef void(__cdecl *IDllFunction0)(IDllScriptComponent*,IDllScriptComponent::Func0);
typedef void(__cdecl *IDllFunction1)(IDllScriptComponent*,IDllScriptComponent::Func1, GameObject);
typedef void(__cdecl *InitIGame)(IGame*);

#include "Game/Script/SGame.h"
SGame gSGame;

#ifdef _ENGINE_MODE

void CreateScriptFileExtension(const std::string& classNmae, const std::string& extension){

	std::fstream file;

	auto srcname = "EngineResource/ScriptTemplate/ScriptTemplate" + extension;
	file.open(srcname, std::ios::in | std::ios::ate);

	std::fstream outFile;
	auto outfilename = "ScriptComponent/Scripts/"+ classNmae + extension;
	outFile.open(outfilename, std::ios::out);

	int length = (int)file.tellg();
	file.seekg(0, file.beg);//�t�@�C���|�C���^���ŏ��ɖ߂�
	char * buf = new char[length+1];

	memset(buf, 0, (length+1)*sizeof(char));
	file.read(buf, length);

	std::string buffer = buf;
	std::string from = "$temp$";
	std::string to = classNmae;

	auto pos = buffer.find(from);
	while (pos != std::string::npos){
		buffer.replace(pos, from.size(), to);
		pos = buffer.find(from, pos + to.size());
	}

	outFile.write(buffer.c_str(), buffer.size());

	file.close();
	outFile.close();

	delete[] buf;
}


bool removeDirectory(std::string fileName)
{
	bool retVal = true;
	std::string nextFileName;

	WIN32_FIND_DATA foundFile;

	HANDLE hFile = FindFirstFile((fileName + "\\*.*").c_str(), &foundFile);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//If a found file is . or .. then skip
			if (strcmp(foundFile.cFileName, ".") != 0 && strcmp(foundFile.cFileName, "..") != 0)
			{
				//The path should be absolute path
				nextFileName = fileName + "\\" + foundFile.cFileName;

				//If the file is a directory
				if ((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					removeDirectory(nextFileName.c_str());
					RemoveDirectory(nextFileName.c_str());
				}
				//If the file is a file
				else
				{
					DeleteFile(nextFileName.c_str());
				}
			}
		} while (FindNextFile(hFile, &foundFile) != 0);
	}

	FindClose(hFile);

	//Delete starting point itseft
	if (RemoveDirectory(fileName.c_str()) == 0)retVal = false;

	return retVal;
}
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

std::string outConsole;
bool create_cmd_process(){
	//	�p�C�v�̍쐬
	HANDLE readPipe;
	HANDLE writePipe;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (CreatePipe(&readPipe, &writePipe, &sa, 0) == 0){
		Window::AddLog("�p�C�v���쐬�ł��܂���ł���");
		return false;
	}

	Window::AddLog("�R���p�C���J�n...");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = writePipe;
	si.hStdError = writePipe;
	si.wShowWindow = SW_HIDE;
#ifdef _DEBUG
	char cmd[] = "cmd.exe /K \".\\ScriptComponent\\createdll_auto_d.bat\"";
#else
	char cmd[] = "cmd.exe /K \".\\ScriptComponent\\createdll_auto.bat\"";
#endif

	if (!PathFileExists("ScriptComponent\\createdll_auto.bat")){
		Window::AddLog("�R���p�C�����s");
		return false;
	}

	//	�v���Z�X�̋N��(cmd.exe)
	if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == 0){
		Window::AddLog("�v���Z�X�̍쐬�Ɏ��s���܂���");
		return false;
	}
	HANDLE childProcess = pi.hProcess;
	CloseHandle(pi.hThread);

	outConsole.reserve(20000);
	char readBuf[1025];
	bool end = false;
	do{
		DWORD totalLen, len;
		if (WaitForSingleObject(childProcess, 100) == WAIT_OBJECT_0)
			end = true;
		if (PeekNamedPipe(readPipe, NULL, 0, NULL, &totalLen, NULL) == 0)
			break;
		if (0 < totalLen){
			if (ReadFile(readPipe, readBuf, sizeof(readBuf) - 1, &len, NULL) == 0){
				Window::AddLog("�G���[");
				return false;
			}
			readBuf[len] = 0;

			//if (sizeof(mem) - 1<memSz + len){	//�����������ӂ�Ȃ��悤�ɃN���A����
			//	mem[0] = 0;
			//	memSz = 0;
			//}
			//memSz += len;
			//strcat_s(mem, sizeof(mem), readBuf);
			outConsole += readBuf;
			//SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);	//	�X�N���[���o�[����ԉ��ֈړ�������
			if (totalLen>len)	//	�v���Z�X�͏I�����Ă��邪�܂��f�[�^���[���c���Ă���̂ŏI����ۗ�
				end = false;

			{
				auto out = forward_than_find_first_of(outConsole, "\n");
				if (out == "")continue;
				outConsole = behind_than_find_first_of(outConsole, "\n");
				out.pop_back();
				Window::AddLog(out);
			}
		}
	} while (end == false);

	if (CloseHandle(writePipe) == 0){
		Window::AddLog("�p�C�v����邱�Ƃ��ł��܂���ł����B");
		return false;
	}
	if (CloseHandle(readPipe) == 0){
		Window::AddLog("�p�C�v����邱�Ƃ��ł��܂���ł����B");
		return false;
	}
	CloseHandle(pi.hProcess);
	for (int i = 0; i < 100; i++){
		auto out = forward_than_find_first_of(outConsole, "\n");
		outConsole = behind_than_find_first_of(outConsole, "\n");
		if (out == "")break;
		out.pop_back();
		Window::AddLog(out);
	}
	Window::AddLog("�R���p�C���I��");
	return true;
}

#endif

//����DLL�������Ă�R���|�[�l���g���Ǘ�
class UseScriptActors{
public:
	UseScriptActors(){
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		mFunction0 = NULL;
		mFunction1 = NULL;


#ifdef _ENGINE_MODE
#else
		DllLoad();
#endif

	}
	~UseScriptActors(){
		UnLoad();
	}

#ifdef _ENGINE_MODE
	void ReCompile(){
		for (auto& p : mList){
			p->SaveParam();
			p->Unload();
		}

		UnLoad();

		CreateIncludeClassFile();

		
		//�Ō�̃R���p�C���o�[�W�����m�F
		const std::string LogFilePath = "Temp/buildver.txt";
		int ver = _BUILD_VERSION;
		int buildver = 0;
		{
			std::ifstream in(LogFilePath, std::ios::in);
			in >> buildver;
		}
		//�o�[�W�������Ⴄ
		if (ver != buildver){
			std::ofstream out(LogFilePath, std::ios::out);
			out << ver;
			//�����[�X�t�H���_�폜
			removeDirectory(".\\ScriptComponent\\Release");
		}

		if (!create_cmd_process()){
			//MessageBox(Window::GetMainHWND(), "�r���h���蓮�ōs���ĉ������B", "DLL�ǂݍ���", MB_OK);

					char cdir[255];
					GetCurrentDirectory(255, cdir);
					std::string  pass = cdir;
			#ifdef _DEBUG
					pass += "/ScriptComponent/createdll_auto_d.bat";
			#else
					pass += "/ScriptComponent/createdll_auto_.bat";
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
			
			CloseHandle(sei.hProcess);
		}

		DllLoad();

		for (auto& p : mList){
			p->Load();
			p->LoadParam();
		}
	}

	void CreateIncludeClassFile(){
		//�t�@�C�����X�V���ăR���p�C��������
		{
			std::ofstream h1("./ScriptComponent/Script.cpp", std::ios::app | std::ios::out);
			h1 << "";
			h1.close();
		}
		{

			std::ofstream h1("./ScriptComponent/Reflection/ReflectionSetter.cpp", std::ios::app | std::ios::out);
			h1 << "";
			h1.close(); 
		}
		//�C���N���[�h�̍\�z
		File includesFile("./ScriptComponent/System/include");
		includesFile.FileCreate();
		File factorysFile("./ScriptComponent/System/factory");
		factorysFile.FileCreate();
		File reflectionsFile("./ScriptComponent/System/reflection");
		reflectionsFile.FileCreate();

		namespace sys = std::tr2::sys;
		sys::path p("./ScriptComponent/Scripts/"); // �񋓂̋N�_
		//std::for_each(sys::directory_iterator(p), sys::directory_iterator(),
			//  �ċA�I�ɑ�������Ȃ�R�`����
		std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
			[&](const sys::path& p) {
			if (sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
				if (p.extension() == ".h"){
					auto out = "\n#include\"../" + p.generic_string() + "\"";
					includesFile.Out(out);

					auto outf = "\n_ADD(" + p.stem().string() + ");";
					factorysFile.Out(outf);

					findSerialize(&reflectionsFile, p.stem().string());
				}
			}
			else if (sys::is_directory(p)) { // �f�B���N�g���Ȃ�...
				//std::cout << "dir.: " << p.string() << std::endl;
			}
		});
	}

	void findSerialize(File* file, const std::string& classname){

		auto filepath = "./ScriptComponent/Scripts/" +classname+".h";
		std::ifstream classfile(filepath, std::ios::in);

		if (classfile.fail())
		{
			return;
		}
		//�S���ǂݍ���
		std::istreambuf_iterator<char> it(classfile);
		std::istreambuf_iterator<char> last;
		std::string str(it, last);

		unsigned int findpoint = 0;
		for(;;){
			//�V���A���C�Y����
			auto serialize = str.find("SERIALIZE", findpoint);

			if (std::string::npos == serialize){
				break;
			}

			auto serializeend = str.find(";", serialize);
			if (std::string::npos == serializeend){
				break;

			}
			findpoint = serializeend;

			//�V���A���C�Y�͈͂𔲂��o��
			auto sub = serializeend - serialize;
			auto serializestr = str.substr(serialize, sub);

			findMember(std::move(serializestr), file, classname);
		}
	}

	void findMember(std::string&& pickstr, File* file, const std::string& classname){

		//�����ɕ���킵������ȕ������폜
		filter(pickstr);


		//�X�y�[�X��؂�ɂ���
		std::vector<std::string> v;
		split(v, pickstr, " ");
		auto num = (int)v.size();

		//������̌���Member�ϐ�������
		std::string member;
		for (int i = num - 1; i >= 0; i--){
			auto l = (int)v[i].size();
			if (l == 0)continue;
			member = v[i];
			break;
		}

		if (member != ""){
			reflect(member,file, classname);
		}
	}

	void reflect(const std::string& member, File* file, const std::string& classname){
		auto outf = "\n_REF(" + classname + "," + member + ");";
		file->Out(outf);
	}

	void filter(std::string& str){
		//while(1){
		//	auto temp = str.find("\\\\");
		//	if (temp == std::string::npos){
		//		break;
		//	}
		//	str.replace(temp,2,"  ");
		//}
		//while (1){
		//	auto temp = str.find("\\\"");
		//	if (temp == std::string::npos){
		//		break;
		//	}
		//	str.replace(temp, 2, "  ");
		//}
		//������Ă��邩

		for(;;){
			auto start = str.find('\"');
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\"', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}

		
		for (;;){
			auto start = str.find("/*");
			if (start == std::string::npos){
				break;
			}
			auto end = str.find("*/", start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}
		for (;;){
			auto start = str.find("//");
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\n', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}

		for (;;){
			auto temp = str.find('\n');
			if (temp == std::string::npos){
				break;
			}
			str.replace(temp, 1, " ");
		}
		for (;;){
			auto temp = str.find('\t');
			if (temp == std::string::npos){
				break;
			}
			str.replace(temp, 1, " ");
		}
		for (;;){
			auto start = str.find('\'');
			if (start == std::string::npos){
				break;
			}
			auto end = str.find('\'', start);
			if (start == std::string::npos){
				str.erase(start);
			}
			else{
				str.erase(start, end);
			}
		}


		//������Ă��邩
		auto equal = str.find('=');
		if (std::string::npos != equal){
			str.erase(equal);
		}

	}

	void split(std::vector<std::string> &v, const std::string &input_string, const std::string &delimiter)
	{
		std::string::size_type index = input_string.find_first_of(delimiter);

		if (index != std::string::npos) {
			v.push_back(input_string.substr(0, index));
			split(v, input_string.substr(index + 1), delimiter);
		}
		else {
			v.push_back(input_string);
		}
	}
#endif

	void UnLoad(){
		FreeLibrary(hModule);
		hModule = NULL;
		mCreate = NULL;
		mDelete = NULL;
		mGetReflect = NULL;
		mFunction0 = NULL;
		mFunction1 = NULL;
		Reflection::map = NULL;
	}
	void DllLoad(){

#ifdef _ENGINE_MODE
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


#else
		hModule = LoadLibrary("ScriptComponent/ExeCompile/ScriptComponent.dll");
#endif
		if (!hModule){
			_SYSTEM_LOG_ERROR("�X�N���v�gDLL�̓ǂݍ���");
			return;
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

		mFunction0 = (IDllFunction0)GetProcAddress(hModule, "Function0");
		if (mFunction0 == NULL)
		{
			UnLoad();
			return;
		}
		mFunction1 = (IDllFunction0)GetProcAddress(hModule, "Function1");
		if (mFunction1 == NULL)
		{
			UnLoad();
			return;
		}
		InitIGame initIGame = (InitIGame)GetProcAddress(hModule, "InitializeIGame");
		if (initIGame == NULL)
		{
			UnLoad();
			return;
		}
		initIGame(&gSGame);
		Reflection::map = ((GetReflectionData_)mGetReflect)();
	}
	IDllScriptComponent* Create(const std::string& ClassName){
		if (!mCreate){
			_SYSTEM_LOG_ERROR("�X�N���v�g[" + ClassName + "]�̍쐬");
			return NULL;
		}
		//dll�ō쐬�����N���X�C���X�^���X���쐬����
		return ((CreateInstance_)mCreate)(ClassName.c_str());
	}

	void Deleter(IDllScriptComponent* script){
		((DeleteInstance_)mDelete)(script);
	}
	void Function(IDllScriptComponent* com,IDllScriptComponent::Func0 func){
		((IDllFunction0)mFunction0)(com,func);
	}
	void Function(IDllScriptComponent* com,IDllScriptComponent::Func1 func, GameObject tar){
		((IDllFunction1)mFunction1)(com,func, tar);
	}
	std::list<ScriptComponent*> mList;
	void* mCreate;
	void* mDelete;
	void* mGetReflect;
	void* mFunction0;
	void* mFunction1;
	HMODULE hModule;

};

UseScriptActors actors;


#ifdef _ENGINE_MODE
//static
void ScriptManager::ReCompile(){
	actors.ReCompile();
}
void ScriptManager::CreateScriptFile(const std::string& className){
	CreateScriptFileExtension(className, ".h");
	CreateScriptFileExtension(className, ".cpp");
}
#endif



ScriptComponent::ScriptComponent(){
	mEndInitialize = false;
	mEndStart = false;
	pDllClass = NULL;
	mSaveParam = NULL;
}
ScriptComponent::~ScriptComponent(){
	if (mSaveParam){
		delete mSaveParam;
		mSaveParam = NULL;
	}
}
void ScriptComponent::Initialize(){
	mCollideMap.clear();
	Load();
	actors.mList.push_back(this);

	if (pDllClass){
		pDllClass->gameObject = gameObject;
	}

#ifdef _ENGINE_MODE
	if (Game::IsGamePlay()){
		Initialize_Script();
	}
#else
	Initialize_Script();
#endif
}

void ScriptComponent::Start(){
#ifdef _ENGINE_MODE
	if (Game::IsGamePlay()){
		Start_Script();
	}

#else
	Start_Script();
#endif
}
void ScriptComponent::Initialize_Script(){

	if (!mEndInitialize){
		mEndInitialize = true;
		actors.Function(pDllClass, &IDllScriptComponent::Initialize);
	}

}

void ScriptComponent::Start_Script(){

	if (!mEndStart){
		mEndStart = true;
		actors.Function(pDllClass, &IDllScriptComponent::Start);
	}
}
void ScriptComponent::Load(){
	if (pDllClass)return;

	//dll�ō쐬�����N���X�C���X�^���X���쐬����
	pDllClass = actors.Create(mClassName);

	if (pDllClass){
		pDllClass->gameObject = gameObject;
	}
}
void ScriptComponent::Unload(){

	if (pDllClass){
		if (mEndInitialize)
			actors.Function(pDllClass, &IDllScriptComponent::Finish);
		actors.Deleter(pDllClass);
	}

	pDllClass = NULL;

	mEndInitialize = false;
	mEndStart = false;

}
void ScriptComponent::ReCompile(){

	Unload();
	Load();
}

void ScriptComponent::SaveParam(){

	if (mSaveParam){
		delete mSaveParam;
		mSaveParam = NULL;
	}
	mSaveParam = new picojson::value();
	shared_ptr<I_InputHelper> prefab_io(NULL);
	I_ioHelper* io = new MemoryOutputHelper(*mSaveParam, prefab_io.Get());
	IO_Data(io);
	delete io;

}
void ScriptComponent::LoadParam(){
	if (mSaveParam){
		shared_ptr<I_InputHelper> prefab_io(NULL);
		I_ioHelper* io = new MemoryInputHelper(*mSaveParam, prefab_io.Get());
		IO_Data(io);
		delete io;
		delete mSaveParam;
		mSaveParam = NULL;
	}
}
int count = 0;
void ScriptComponent::Update(){

	if (pDllClass){

		if (pDllClass){
			pDllClass->gameObject = gameObject;
		}

		actors.Function(pDllClass, &IDllScriptComponent::Update);

		for (auto ite = mCollideMap.begin(); ite != mCollideMap.end();){

			auto& data = *ite;

			if (data.second.HitCount == 0){

				if (data.second.State != ColliderState::Begin){	
					actors.Function(pDllClass, &IDllScriptComponent::OnCollideExit, data.second.Target);
				}
				ite = mCollideMap.erase(ite);
				continue;

			}else if (data.second.State == ColliderState::Begin){
				actors.Function(pDllClass, &IDllScriptComponent::OnCollideBegin, data.second.Target);
				data.second.State = Enter;
			}
			else if (data.second.State == ColliderState::Enter){
				actors.Function(pDllClass, &IDllScriptComponent::OnCollideEnter, data.second.Target);

			}
			//else if (data.second.State == ColliderState::Exit){
			//	actors.Function(pDllClass, &IDllScriptComponent::OnCollideExit, data.second.Target);
			//	ite = mCollideMap.erase(ite);
			//	continue;
			//}
			++ite;
		}
	}

}
void ScriptComponent::Finish(){

	Unload();
	actors.mList.remove(this);
}


void ScriptComponent::OnCollide(Actor* target){
	auto& get = mCollideMap[(int)target];
	if (get.Target){
		get.HitCount++;
		return;
	}

	mCollideMap[(int)target] = ColliderStateData(target->shared_from_this(),ColliderState::Begin,1);

	//if (pDllClass){
	//	actors.Function(pDllClass, &IDllScriptComponent::OnCollideBegin, target);
	//}
}

void ScriptComponent::LostCollide(Actor* target){
	auto ite = mCollideMap.find((int)target);
	if (ite == mCollideMap.end()){
		mCollideMap[(int)target] = ColliderStateData(target->shared_from_this(), ColliderState::Begin, -1);
		return;
	}
	auto& get = *ite;
	if (get.second.Target){
		get.second.HitCount--;
	}


	//mCollideMap[(int)target] = ColliderStateData(target, ColliderState::Exit);

	//mCollideMap.erase((int)target);
	//if (pDllClass){
	//	actors.Function(pDllClass, &IDllScriptComponent::OnCollideExit, target);
	//}
}


#ifdef _ENGINE_MODE

template<class T>
bool reflect(MemberInfo& info, Inspector& ins){

	if (info.GetTypeName() != typeid(T).name()){
		return false;
	}

	volatile T* paramdata = Reflection::Get<T>(info);

	std::function<void(T)> collback = [paramdata](T f){
		*(T*)paramdata = f;
	};
	ins.Add(info.GetName(), (T*)paramdata, collback);

	return true;
}
bool reflect_v(MemberInfo& info, Inspector& ins) {

	if (info.GetTypeName() != typeid(XMVECTOR).name()) {
		return false;
	}

	volatile XMVECTOR* paramdata = Reflection::Get<XMVECTOR>(info);

	Vector3 v(*(XMVECTOR*)paramdata);
	ins.Add(info.GetName(), &v, [paramdata](Vector3 f) {
		paramdata->x = f.x;
		paramdata->y = f.y;
		paramdata->z = f.z;
	});

	return true;
}
bool reflect_g(MemberInfo& info, Inspector& ins) {
	
	if (info.GetTypeName() != typeid(GameObject).name()) {
		return false;
	}

	volatile GameObject* paramdata = Reflection::Get<GameObject>(info);

	ins.Add(info.GetName(), (GameObject*)paramdata, []() {});

	return true;
}

template<class T>
bool reflect_a(MemberInfo& info, Inspector& ins) {

	if (info.GetTypeName() != typeid(T).name()) {
		return false;
	}

	volatile T* paramdata = Reflection::Get<T>(info);

	ins.Add(info.GetName(), (T*)paramdata, []() {});

	return true;
}


#endif

void initparam(int* p){
	*p = 0;
}
void initparam(float* p){
	*p = 0.0f;
}
void initparam(bool* p){
	*p = false;
}
void initparam(std::string* p){
	*p = "";
}
void initparam(XMVECTOR* p) {
	*p = XMVectorSet(0,0,0,0);
}
void initparam(GameObject* p) {
	*p = NULL;
}
void initparam(IAsset* p) {
	p->m_Hash.key_i[0] = 0;
	p->m_Hash.key_i[1] = 0;
	p->m_Hash.key_i[2] = 0;
	p->m_Hash.key_i[3] = 0;
	p->m_Name = "";
}


template<class T>
bool reflect_io(MemberInfo& info,I_ioHelper* io){

	if (info.GetTypeName() != typeid(T).name()){
		return false;
	}

	T* paramdata = Reflection::Get<T>(info);

	if (io->isInput()){
		initparam(paramdata);
	}

	io->func(*paramdata, info.GetName().c_str());

	return true;
}

bool reflect_io_v(MemberInfo& info, I_ioHelper* io) {

	if (info.GetTypeName() != typeid(XMVECTOR).name()) {
		return false;
	}

	XMVECTOR* paramdata = Reflection::Get<XMVECTOR>(info);

	if (io->isInput()) {
		initparam(paramdata);
	}

	io->func(paramdata->x, (info.GetName() + "x").c_str());
	io->func(paramdata->y, (info.GetName() + "y").c_str());
	io->func(paramdata->z, (info.GetName() + "z").c_str());
	io->func(paramdata->w, (info.GetName() + "w").c_str());

	return true;
}


bool reflect_io_g(MemberInfo& info, I_ioHelper* io, Component* com) {

	if (info.GetTypeName() != typeid(GameObject).name()) {
		return false;
	}

	GameObject* paramdata = Reflection::Get<GameObject>(info);

	if (io->isInput()) {
		initparam(paramdata);
	}

	ioGameObjectHelper::func(paramdata, info.GetName().c_str(), io, &com->gameObject);


	return true;
}

template<class T>
bool reflect_io_a(MemberInfo& info, I_ioHelper* io, Component* com) {

	if (info.GetTypeName() != typeid(T).name()) {
		return false;
	}

	T* paramdata = Reflection::Get<T>(info);

	if (io->isInput()) {
		initparam((IAsset*)paramdata);
	}

	io->func(*(IAsset*)paramdata, info.GetName().c_str());

	if (io->isInput()) {
		
		AssetLoad::Instance(paramdata->m_Hash, paramdata);
	}

	return true;
}

#ifdef _ENGINE_MODE
void ScriptComponent::CreateInspector(){


	std::function<void(std::string)> collback = [&](std::string name){
		Window::ClearInspector();
		mClassName = name;
		ReCompile();
	};

	Inspector ins("Script",this);
	ins.AddEnableButton(this);
	ins.Add("Class", &mClassName, collback);

	if (Reflection::map){
		auto infos = Reflection::GetMemberInfos(pDllClass, mClassName);

		for (auto i : infos){
			auto info = Reflection::GetMemberInfo(infos,i);
			bool fal = false;
			do{
				if (reflect<float>(info, ins))break;
				if (reflect<int>(info, ins))break;
				if (reflect<bool>(info, ins))break;
				if (reflect<std::string>(info, ins))break;
				if (reflect_v(info, ins))break;
				if (reflect_g(info, ins))break;
				if (reflect_a<MetaAsset>(info, ins))break;
				if (reflect_a<MeshAsset>(info, ins))break;
				if (reflect_a<BoneAsset>(info, ins))break;
				if (reflect_a<PrefabAsset>(info, ins))break;
				if (reflect_a<ShaderAsset>(info, ins))break;
				if (reflect_a<TextureAsset>(info, ins))break;
				if (reflect_a<PhysxMaterialAsset>(info, ins))break;
				if (reflect_a<SoundAsset>(info, ins))break;
				if (reflect_a<MovieAsset>(info, ins))break;
			} while (fal);
		}

	}

	ins.Complete();
}
#endif

void ScriptComponent::IO_Data(I_ioHelper* io){

	Enabled::IO_Data(io);

#define _KEY(x) io->func( x , #x)
	_KEY(mClassName);
	Load();

	if (pDllClass){
		if (Reflection::map){
			auto infos = Reflection::GetMemberInfos(pDllClass, mClassName);

			for (auto i : infos){
				auto info = Reflection::GetMemberInfo(infos, i);
				bool fal = false;
				do{
					if (reflect_io<float>(info, io))break;
					if (reflect_io<int>(info, io))break;
					if (reflect_io<bool>(info, io))break;
					if (reflect_io<std::string>(info, io))break;
					if (reflect_io_v(info, io))break;
					if (reflect_io_g(info, io, this))break;
					if (reflect_io_a<MetaAsset>(info, io, this))break;
					if (reflect_io_a<MeshAsset>(info, io, this))break;
					if (reflect_io_a<BoneAsset>(info, io, this))break;
					if (reflect_io_a<PrefabAsset>(info, io, this))break;
					if (reflect_io_a<ShaderAsset>(info, io, this))break;
					if (reflect_io_a<TextureAsset>(info, io, this))break;
					if (reflect_io_a<PhysxMaterialAsset>(info, io, this))break;
					if (reflect_io_a<SoundAsset>(info, io, this))break;
					if (reflect_io_a<MovieAsset>(info, io, this))break;
				} while (fal);
			}
		}
	}

#undef _KEY
}