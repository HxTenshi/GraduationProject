#pragma once
#include "AssetDataBase.h"
#include "Library/MD5.h"

namespace AssetLoad{

	template <class T>
<<<<<<< HEAD
	static void Instance(const MD5::MD5HashCoord hash, Asset<T>& asset) {
=======
	static void Instance(const MD5::MD5HashCode hash, Asset<T>& asset) {
>>>>>>> develop
		asset.Free();
		shared_ptr < AssetDataTemplate<T> > temp;
		AssetDataBase::Instance(hash, temp);

		if (temp) {
			asset.m_Ptr = temp;
			asset.m_Hash = hash;
			asset.m_Name = temp->GetFileName();
		}
	}
	template <class T>
<<<<<<< HEAD
	static void Instance(const MD5::MD5HashCoord hash, Asset<T> *asset) {
=======
	static void Instance(const MD5::MD5HashCode hash, Asset<T> *asset) {
>>>>>>> develop
		asset->Free();
		shared_ptr < AssetDataTemplate<T> > temp;
		AssetDataBase::Instance(hash, temp);

		if (temp){
			asset->m_Ptr = temp;
			asset->m_Hash = hash;
			asset->m_Name = temp->GetFileName();
		}
	}
}