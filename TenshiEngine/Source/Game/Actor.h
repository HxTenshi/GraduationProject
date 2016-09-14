#pragma once

#include <d3d11.h>
#include "XNAMath/xnamath.h"
#include <functional>
#include <queue>
#include "Game/Component/ComponentList.h"
#include "Game/Parts/Enabled.h"

#include "Engine/AssetDataBase.h"
#include "Types.h"

class ITransformComponent;
class File;
class PrefabAssetData;
//PhysX�e�X�g�p
namespace physx{
	class PxTransform;
}

class IActor{
public:
	virtual ~IActor(){}
	virtual void* _GetScript(const char* name) = 0;
};

class Actor 
	: public IActor
	, public Enabled
	, public enable_shared_from_this<Actor>{
public:
	Actor();
	virtual ~Actor();
	virtual void Initialize();
	virtual void Start();
	virtual void Finish();

#ifdef _ENGINE_MODE
	virtual void Initialize_Script();
	virtual void Start_Script();
	virtual void EngineUpdateComponent(float deltaTime);
#endif
	virtual void UpdateComponent(float deltaTime);
	virtual void Update(float deltaTime);

	void SetUpdateStageCollQueue(const std::function<void()> coll);

	template<class T>
	weak_ptr<T> GetComponent(){
		return mComponents.GetComponent<T>();
	}
	weak_ptr<Component> GetComponent(const size_t& hash){
		return mComponents.GetComponent(hash);
	}

	template<class T>
	T* GetScript(){
		return (T*)_GetScript(typeid(T).name());
	}

	void* _GetScript(const char* name) override;

	template<class T>
	shared_ptr<T> AddComponent(shared_ptr<T> component){
		return mComponents.AddComponent<T>(component);
	}
	template<class T>
	shared_ptr<T> AddComponent(){
		return mComponents.AddComponent<T>();
	}
	template<class T>
	void RemoveComponent(){
		mComponents.RemoveComponent<T>();
	}
	void RemoveComponent(Component* comptr){
		mComponents.RemoveComponent(comptr);
	}

#ifdef _ENGINE_MODE
	virtual void CreateInspector();
#endif

	std::string Name(){return mName;}
	void Name(const std::string& name){mName = name;}


	std::string Prefab(){ return mPrefab; }

	UniqueID GetUniqueID(){
		return mUniqueHash;
	}


	//�y�A�����g�ύX�R�[���o�b�N�����s
	void RunChangeParentCallback();

	void PastePrefabParam(picojson::value& json);

	//void ExportSceneDataStart(const std::string& pass, File& sceneFile);
	//void ExportSceneData(const std::string& pass, File& sceneFile);
	void ExportData(const std::string& pass, const std::string& fileName, bool childExport = false);
	//void ExportData(const std::string& pass);
	void ImportData(const std::string& fileName);
	void ImportData(picojson::value& json);
	bool ImportDataAndNewID(const std::string& fileName);

	void ExportData(picojson::value& json, bool childExport=false);
	void ImportDataAndNewID(picojson::value& json);

	void CreateNewID();

	shared_ptr<ITransformComponent> mTransform;

	void* mTreeViewPtr;

	int GetLayer(){
		return mPhysxLayer;
	}

protected:
	ComponentList mComponents;

protected:


	virtual void _ExportData(I_ioHelper* io, bool childExport=false);
	virtual void _ImportData(I_ioHelper* io);

	std::queue<std::function<void()>> mUpdateStageCollQueue;
	std::string mName;

	std::string mPrefab;
	PrefabAssetDataPtr mPrefabAsset;

	UniqueID mUniqueHash;
	bool mEndStart;

	int mPhysxLayer;


private:
	virtual void OnEnabled()override;
	virtual void OnDisabled()override;
};