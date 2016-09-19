#pragma once

class IAssetDataTemplate;
#include "Library/MD5.h"
#include "MySTL/ptr.h"

class IAsset{
public:
	IAsset(){
		m_Ptr = NULL;
		memset(&m_Hash, 0, sizeof(m_Hash));
		m_Name = "";
	}
	virtual ~IAsset(){}
	virtual void Load(MD5::MD5HashCoord hash){
		(void)hash;
	}
	void Free(){
		m_Ptr = NULL;
		memset(&m_Hash, 0, sizeof(m_Hash));
		m_Name = "";
	}
	bool IsLoad()const{
		return m_Ptr != NULL;
	}
	shared_ptr <IAssetDataTemplate> m_Ptr;
	MD5::MD5HashCoord m_Hash;
	std::string m_Name;
};


template<class T>
class Asset : public IAsset {
public:
	virtual ~Asset(){}
	void Load(MD5::MD5HashCoord hash) override{
		Free();
		shared_ptr<AssetDataTemplate<T>> temp;
		AssetDataBase::Instance(hash, temp);
		if (temp){
			m_Ptr = temp;
			m_Hash = hash;
			m_Name = temp->GetFileName();

		}
	}

	T* Get() const{
		return (T*)((shared_ptr<AssetDataTemplate<T>>)m_Ptr)->GetFileData();
	}

	T* Get(){
		return (T*)((shared_ptr<AssetDataTemplate<T>>)m_Ptr)->GetFileData();
	}
};