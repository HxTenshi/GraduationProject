#pragma once


#include "MySTL/ptr.h"
#include <functional>
#include <unordered_map>
#include <map>
#include <string>

enum class AssetFileType{
	None,
	Meta,
	Prefab,
	Temesh,
	Tebone,
	Shader,
	Vmd,
	Image,
	PhysxMaterial,
	Sound,
	Movie,
	Count,
};

class IAssetDataTemplate{
public:
	IAssetDataTemplate(AssetFileType type = AssetFileType::None)
		:m_AssetFileType(type){
	}
	virtual ~IAssetDataTemplate(){}
	virtual void CreateInspector() = 0;
	virtual void FileUpdate(const char* filename) = 0;
	virtual std::string GetFileName() = 0;

	static const AssetFileType _AssetFileType = AssetFileType::None;
	const AssetFileType m_AssetFileType;
private:
	IAssetDataTemplate(const IAssetDataTemplate &);
	IAssetDataTemplate& operator=(const IAssetDataTemplate &);
};

using AssetDataTemplatePtr = shared_ptr < IAssetDataTemplate >;


class AssetFactory{
public:
	static AssetDataTemplatePtr Create(const char* filename);
private:
	static std::unordered_map<std::string, std::function<AssetDataTemplatePtr(const char*)>> m_Factory;
protected:
	AssetFactory();
	~AssetFactory();
};
#include "Library/MD5.h"
class AssetDataBase{
public:
	
	template <class T>
	static void Instance(const char* filename, shared_ptr<T>& out){

		auto file = m_AssetCache.find(filename);


		AssetDataTemplatePtr data;

		//���^�t�@�C�������݂��Ȃ�
		if (file == m_AssetCache.end()){

			data = AssetFactory::Create(filename);
			if (data){
				MD5::MD5HashCoord hash;
				memset(hash.key_c, NULL, sizeof(MD5::MD5HashCoord));
				m_AssetCache.insert(std::make_pair(filename, std::make_pair(hash, data)));
			}
		}
		//�����[�h
		else if (file->second.second == NULL){
			data = AssetFactory::Create(filename);
			if (data){
				file->second.second = data;
				auto temp = m_AssetMetaCache.find(file->second.first);
				if (temp != m_AssetMetaCache.end()){
					temp->second.second = data;
				}
			}
		}
		//���[�h�ς�
		else{
			data = file->second.second;
		}
		
		if (data && 
			(T::_AssetFileType == data->m_AssetFileType) ||
			T::_AssetFileType == AssetFileType::None){
			out = data;
		}
	}

	template <class T>
	static void Instance(const MD5::MD5HashCoord& hash, shared_ptr<T>& out){

		auto file = m_AssetMetaCache.find(hash);


		AssetDataTemplatePtr data;

		if (file == m_AssetMetaCache.end()){

		}
		else{
			if (file->second.second){
				data = file->second.second;
			}
			else{
				//�����[�h
				data = AssetFactory::Create(file->second.first.c_str());
				file->second.second = data;
				auto temp = m_AssetCache.find(file->second.first);
				if (temp != m_AssetCache.end()){
					temp->second.second = data;
				}
			}
		}

		if (data &&
			(T::_AssetFileType == data->m_AssetFileType) ||
			T::_AssetFileType == AssetFileType::None){
			out = data;
		}
	}

	static bool FilePath2Hash(const char* filename, MD5::MD5HashCoord& hash){

		auto file = m_AssetCache.find(filename);
		if (file == m_AssetCache.end())return false;

		hash = file->second.first;
		return true;
	}
	static bool Hash2FilePath(MD5::MD5HashCoord& hash, std::string& filename){

		auto file = m_AssetMetaCache.find(hash);
		if (file == m_AssetMetaCache.end())return false;

		filename = file->second.first;
		return true;
	}

	static void InitializeMetaData(const char* filename);

	static void Remove(const char* filename){

		auto file = m_AssetCache.find(filename);
		if (file == m_AssetCache.end())return;

		auto hashfile = m_AssetMetaCache.find(file->second.first);
		if (hashfile != m_AssetMetaCache.end()){
			m_DeleteCache = hashfile->second;
			m_AssetMetaCache.erase(hashfile);
		}
		
		m_AssetCache.erase(file);
		

	}

	static void FileUpdate(const char* filename){

		auto file = m_AssetCache.find(filename);
		if (file != m_AssetCache.end()){
			if (file->second.second)
				file->second.second->FileUpdate(filename);
		}
		else{
			//AssetDataTemplatePtr temp;
			//Instance(filename,temp);
		}
	}

	static void CreateInspector(const char* filename){
		AssetDataTemplatePtr temp;

		Instance(filename, temp);
		if (temp){
			temp->CreateInspector();
		}
	}
	static AssetDataTemplatePtr GetAssetFile(const std::string& Filename){
		auto cache = m_AssetCache.find(Filename);
		if (cache == m_AssetCache.end())return NULL;
		return cache->second.second;
	}
	static void MoveAssetFile(const std::string& NewFilename, AssetDataTemplatePtr file){
		InitializeMetaData(NewFilename.c_str());
		auto cache = m_AssetCache.find(NewFilename);

		if (cache == m_AssetCache.end())return;
		cache->second.second = file;
		auto temp = m_AssetMetaCache.find(cache->second.first);
		if (temp != m_AssetMetaCache.end()){
			temp->second.second = file;
		}
	}
	static void AssetFileRename(const std::string& OldName, const std::string& NewName){
		if (OldName == NewName)return;
		auto cache = m_AssetCache.find(OldName);
		if (cache != m_AssetCache.end()){
			auto meta = m_AssetMetaCache.find(cache->second.first);
			if (meta != m_AssetMetaCache.end()){
				meta->second.first = NewName;
			}

			m_AssetCache.insert(std::make_pair(NewName, cache->second));
			m_AssetCache.erase(cache);
		}

	}

private:
	static std::unordered_map<std::string, std::pair<MD5::MD5HashCoord,AssetDataTemplatePtr>> m_AssetCache;
	static std::map<MD5::MD5HashCoord, std::pair<std::string, AssetDataTemplatePtr>> m_AssetMetaCache;
	static std::pair<std::string, AssetDataTemplatePtr> m_DeleteCache;
};


template<class T>
class AssetDataTemplate : public IAssetDataTemplate {
public:

	AssetDataTemplate()
		:IAssetDataTemplate(_AssetFileType){
	}

	static AssetDataTemplatePtr Create(const char* filename);
	static AssetDataTemplatePtr Create(T* fileData);

	virtual ~AssetDataTemplate()override{}
	void CreateInspector()override{}
	void FileUpdate(const char* filename)override;
	std::string GetFileName()override;

	const T* GetFileData();

	static const AssetFileType _AssetFileType;


protected:
	T* m_FileData;

private:
	AssetDataTemplate<T>& operator = (const AssetDataTemplate<T>&) = delete;
	AssetDataTemplate<T>(const AssetDataTemplate<T>&) = delete;

};


class MetaFileData;
using MetaAssetDataPtr = shared_ptr < AssetDataTemplate<MetaFileData> >;
const AssetFileType AssetDataTemplate<MetaFileData>::_AssetFileType = AssetFileType::Meta;
void AssetDataTemplate<MetaFileData>::CreateInspector(){}

class MeshFileData;
using MeshAssetDataPtr = shared_ptr < AssetDataTemplate<MeshFileData> >;
const AssetFileType AssetDataTemplate<MeshFileData>::_AssetFileType = AssetFileType::Temesh;
void AssetDataTemplate<MeshFileData>::CreateInspector();

class BoneFileData;
using BoneAssetDataPtr = shared_ptr < AssetDataTemplate<BoneFileData> >;
const AssetFileType AssetDataTemplate<BoneFileData>::_AssetFileType = AssetFileType::Tebone;
void AssetDataTemplate<BoneFileData>::CreateInspector();

class PrefabFileData;
using PrefabAssetDataPtr = shared_ptr < AssetDataTemplate<PrefabFileData> >;
const AssetFileType AssetDataTemplate<PrefabFileData>::_AssetFileType = AssetFileType::Prefab;
void AssetDataTemplate<PrefabFileData>::CreateInspector();

class ShaderFileData;
using ShaderAssetDataPtr = shared_ptr < AssetDataTemplate<ShaderFileData> >;
const AssetFileType AssetDataTemplate<ShaderFileData>::_AssetFileType = AssetFileType::Shader;
void AssetDataTemplate<ShaderFileData>::CreateInspector();

class TextureFileData;
using TextureAssetDataPtr = shared_ptr < AssetDataTemplate<TextureFileData> >;
const AssetFileType AssetDataTemplate<TextureFileData>::_AssetFileType = AssetFileType::Image;
void AssetDataTemplate<TextureFileData>::CreateInspector(){}

class PhysxMaterialFileData;
using PhysxMaterialAssetDataPtr = shared_ptr < AssetDataTemplate<PhysxMaterialFileData> >;
const AssetFileType AssetDataTemplate<PhysxMaterialFileData>::_AssetFileType = AssetFileType::PhysxMaterial;
void AssetDataTemplate<PhysxMaterialFileData>::CreateInspector();

class SoundFileData;
using SoundAssetDataPtr = shared_ptr < AssetDataTemplate<SoundFileData> >;
const AssetFileType AssetDataTemplate<SoundFileData>::_AssetFileType = AssetFileType::Sound;
void AssetDataTemplate<SoundFileData>::CreateInspector(){}


class MovieFileData;
using MovieAssetDataPtr = shared_ptr < AssetDataTemplate<MovieFileData> >;
const AssetFileType AssetDataTemplate<MovieFileData>::_AssetFileType = AssetFileType::Movie;
void AssetDataTemplate<MovieFileData>::CreateInspector(){}


template <class T>
const AssetFileType AssetDataTemplate<T>::_AssetFileType = AssetFileType::None;

#include "details.h"


#include "IAsset.h"


using MetaAsset = Asset<MetaFileData>;
using MeshAsset = Asset<MeshFileData>;
using BoneAsset = Asset<BoneFileData>;
using PrefabAsset = Asset<PrefabFileData>;
using ShaderAsset = Asset<ShaderFileData>;
using TextureAsset = Asset<TextureFileData>;
using PhysxMaterialAsset = Asset<PhysxMaterialFileData>;
using SoundAsset = Asset<SoundFileData>;
using MovieAsset = Asset<MovieFileData>;