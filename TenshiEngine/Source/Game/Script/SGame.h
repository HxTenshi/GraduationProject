#pragma once

#include "Game/Game.h"
#include "Game/DeltaTime.h"
#include "IGame.h"
#include "PhysX/PhysX3.h"
#include "Engine/Debug.h"
#include "Engine/DebugEngine.h"
#include "Engine/AssetFile/Prefab/PrefabFileData.h"
#include "Game/System.h"
#include "Application/shutdown.h"

//�Q�[���̃X�^�e�B�b�N�֐��̌�����
class SGame : public IGame{
public:
	GameObject GetRootActor()override{
		return Game::GetRootActor();
	}
	Game::GameObjectPtr CreateActor(const char* prefab)override{


		auto a = make_shared<Actor>();

		PrefabAssetDataPtr mPrefabAsset;
		AssetDataBase::Instance(prefab, mPrefabAsset);
		if (mPrefabAsset){
			if (mPrefabAsset->GetFileData()){
				auto val = mPrefabAsset->GetFileData()->GetParam();
				a->ImportDataAndNewID(*val);
				return a;
			}

		}


		if (!a->ImportDataAndNewID(prefab)){
			//delete a;
			return NULL;
		}
		return a;
	}

	GameObject FindActor(const char* name)override{
		return Game::FindNameActor(name);
	}
	void AddObject(Actor* actor) override{
		Game::AddObject(actor);
	}
	void DestroyObject(Actor* actor) override{
		Game::DestroyObject(actor);

	}


	void LoadScene(const std::string& FilePath) override{
		Game::LoadScene(FilePath);
	}
	DebugEngine* Debug() override{
		return &mDebugEngine;
	}

	PhysXEngine* PhysX() override{
		return Game::GetPhysXEngine();
	}

	::DeltaTime* DeltaTime(){
		return Game::GetDeltaTime();
	}
	::System* System(){
		return Game::System();
	}

	void Shutdown(){
#ifdef _ENGINE_MODE
		//Game::ChangePlayGame(false);
#else
		App::Shutdown();
#endif
	}
	
private:
	DebugSystem mDebugEngine;
};
