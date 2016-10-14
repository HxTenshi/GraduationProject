#pragma once

#include "MySTL/ptr.h"
#include "Library/picojson.h"
#include "../AssetFileData.h"

class I_InputHelper;
class MemoryInputHelper;
class Actor;

class PrefabFileData : public AssetFileData{
public:
	PrefabFileData();
	~PrefabFileData();

	bool Create(const char* filename) override;
	picojson::value Apply();

	shared_ptr<I_InputHelper> GetData() const;
	weak_ptr<Actor> GetActor() const{
		return m_PrefabActor;
	}

	picojson::value* GetParam() const{
		return mBeforeParam;
	}


private:
	//�R�s�[�֎~
	PrefabFileData(const PrefabFileData&) = delete;
	PrefabFileData& operator=(const PrefabFileData&) = delete;

	shared_ptr<Actor> m_PrefabActor;
	picojson::value* mBeforeParam;
};