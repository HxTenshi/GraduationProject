#pragma once

#include "MySTL/ptr.h"

enum class AssetFileType{
	Tesmesh,
	Tedmesh,
	Tebone,
	Vmd,
	Image,
	Count,
};

class AssetData{
public:

protected:
	void* m_Data;
	AssetFileType m_AssetFileType;
	char m_GUID[33];

};
using AssetDataPtr = shared_ptr < AssetData > ;



#include <functional>
#include <map>
class AssetFactory{
public:
	static AssetDataPtr Create(const char* filename);
private:
	static std::map<std::string, std::function<AssetDataPtr(const char*)>> m_Factory;
protected:
	AssetFactory();
};

class AssetDataBase{
public:
	
	static AssetDataPtr Instance(const char* filename);

private:

	static std::map<std::string, AssetDataPtr> m_AssetCache;
};




#include "Engine/AssetFile/Mesh/MeshFileData.h"
#include "Engine/AssetFile/Mesh/MeshBufferData.h"

class MeshAssetData : public AssetData{
public:
	~MeshAssetData();

	static AssetDataPtr MeshAssetData::Create(const char* filename);

	const MeshFileData& GetFileData() const;
	const MeshBufferData& GetBufferData() const;

private:

	MeshAssetData();


	//�R�s�[�֎~
	MeshAssetData(const MeshAssetData&) = delete;
	MeshAssetData& operator=(const MeshAssetData&) = delete;

	MeshFileData	m_MeshFileData;		// �t�@�C����ǂݍ��񂾃f�[�^
	MeshBufferData	m_MeshBufferData;	// �`��Ɏg�p����f�[�^

};

using MeshAssetDataPtr = shared_ptr < MeshAssetData >;


#include "Engine/AssetFile/Bone/BoneFileData.h"

class BoneAssetData : public AssetData{
public:
	~BoneAssetData();

	static AssetDataPtr BoneAssetData::Create(const char* filename);

	const BoneFileData& GetFileData() const;

private:

	BoneAssetData();


	//�R�s�[�֎~
	BoneAssetData(const BoneAssetData&) = delete;
	BoneAssetData& operator=(const BoneAssetData&) = delete;

	BoneFileData	m_BoneFileData;		// �t�@�C����ǂݍ��񂾃f�[�^

};

using BoneAssetDataPtr = shared_ptr < BoneAssetData >;