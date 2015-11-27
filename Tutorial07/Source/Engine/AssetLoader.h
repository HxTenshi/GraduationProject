#pragma once

#include <string>
#include "XNAMath/xnamath.h"
#include "Graphic/model/ModelBuffer.h"
#include "Graphic/Material/Material.h"
#include "Graphic/model/ModelBufferPMD.h"
#include <allocators>

#include "Graphic/Loader/Model/PMX.h"

#include "Game/Component.h"

enum class PMXFileVer{
	v0x01 = 0x0001//pos normal tex �P�Â�
};

class ModelData;
using ModelDataPtr = shared_ptr < ModelData > ;

struct PolygonsData{
	using VertexType = SimpleVertex;
	//using IndexType = unsigned int;
	using IndexType = unsigned short;
	using MeshType = int;
	
	std::vector<VertexType> Vertexs;
	std::vector<IndexType> Indices;
	std::vector<MeshType> Meshs;
	//unsigned int VertexNum;
	//unsigned int IndexNum;
	//unsigned int MeshCount;
};

class ModelData{
public:
	~ModelData(){
	}

	static
	ModelDataPtr Create(PolygonsData& m_Polygons){

		ModelData *temp = new ModelData();
		temp->m_Polygons = std::move(m_Polygons);
		temp->CreateBuffer();
		return ModelDataPtr(temp);
	}

private:
	ModelData(){
	}

	void CreateBuffer(){
		//���L�����f�[�^�̏���
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));

		//�o�[�e�b�N�X�̍쐬
		{
			bd.ByteWidth = sizeof(PolygonsData::VertexType) * m_Polygons.Vertexs.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			InitData.pSysMem = m_Polygons.Vertexs.data();
			Device::mpd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
		}
		//�C���f�b�N�X�̍쐬
		{
			bd.ByteWidth = sizeof(PolygonsData::IndexType) * m_Polygons.Indices.size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

			InitData.pSysMem = m_Polygons.Indices.data();
			Device::mpd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
		}
	}

public:
	PolygonsData m_Polygons;

	//����͌��݃��f���o�b�t�@�[������Ă�
	//���㒼��
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
};

class AssetLoader{
public:
	void LoadFile(const std::string& path){

		ExportModel(path);
	}
	ModelDataPtr LoadAsset(const std::string& path){

		return LoadModel(path);
	}


private:
	ModelDataPtr LoadModel(const std::string& path){
		if (path.find(".tesmesh\0") != std::string::npos){
			return LoadTEStaticMesh(path);
		}
		return ModelDataPtr();
	}
	void ExportModel(const std::string& path){
		if (path.find(".pmd\0") != std::string::npos){
		}
		else if (path.find(".pmx\0") != std::string::npos){
			PMX_to_TEStaticMesh(path);
		}
	}
	//template<class T>
	//void write(T& write, FILE *hFP){
	//	fwrite(&write, sizeof(T), 1, hFP);
	//}
	template<class T>
	void vecfwrite(const std::vector<T>& write, FILE *hFP){

		int num = write.size();
		fwrite(&num, sizeof(int), 1, hFP);
		fwrite(write.data(), sizeof(T), write.size(), hFP);
	}
	HRESULT PMX_to_TEStaticMesh(const std::string& path){
		pmx data(path.c_str());

		HRESULT hr = S_OK;
		if (!data.mLoadResult)return S_FALSE;
		std::string name = path;
		unsigned int loc = name.find_last_of("\\");
		if (loc != std::string::npos && (loc + 1) != std::string::npos){
			loc += 1;
			name.erase(0,loc);
		}

		//���b�V���̍쐬
		{

			PolygonsData buffer;
			//���_�f�[�^���擾
			buffer.Vertexs.resize(data.vert_count);

			int i = 0;
			for (auto& v : buffer.Vertexs){
				v.Pos.x = data.vertex_data.vertex[i].v.pos[0];
				v.Pos.y = data.vertex_data.vertex[i].v.pos[1];
				v.Pos.z = data.vertex_data.vertex[i].v.pos[2];
				v.Normal.x = data.vertex_data.vertex[i].v.normal_vec[0];
				v.Normal.y = data.vertex_data.vertex[i].v.normal_vec[1];
				v.Normal.z = data.vertex_data.vertex[i].v.normal_vec[2];
				v.Tex.x = data.vertex_data.vertex[i].v.uv[0];
				v.Tex.y = data.vertex_data.vertex[i].v.uv[1];
				i++;
			}

			//�C���f�b�N�X�f�[�^���擾
			buffer.Indices.resize(data.face_vert_count);

			i = 0;
			for (auto& idx : buffer.Indices){
				idx = ((PolygonsData::IndexType*)data.face_vert_index)[i];
				i++;
			}

			//���b�V���擾
			buffer.Meshs.resize(data.material_count);

			i = 0;
			for (auto& m : buffer.Meshs){
				m = data.material[i].face_vert_count;
				i++;
			}


			std::string savePath = "Assets/" + name + ".tesmesh";

			FILE *hFP;
			fopen_s(&hFP,savePath.c_str(), "wb");
			if (hFP != 0){
				vecfwrite(buffer.Vertexs, hFP);
				vecfwrite(buffer.Indices, hFP);
				vecfwrite(buffer.Meshs, hFP);
				fclose(hFP);
			}
		}

		//�}�e���A���A�Z�b�g�̍쐬
		{
			shared_ptr<MaterialComponent> mc = shared_ptr<MaterialComponent>(new MaterialComponent());
			CreateMaterial(data.material_count, data.material, data.textureName, "Assets/Texture/", mc);
			mc->SaveAssetResource("Assets/" + name + ".txt");
		}

		return hr;
	}

	template<class T>
	void vecfread(std::vector<T>& read, FILE *hFP){

		int num;
		fread(&num, sizeof(int), 1, hFP);
		read.resize(num);
		fread(read.data(), sizeof(T), read.size(), hFP);
	}
	ModelDataPtr LoadTEStaticMesh(const std::string& path){
		FILE *hFP;
		fopen_s(&hFP,path.c_str(), "rb");

		if (hFP == 0)return ModelDataPtr();

		PolygonsData poly;
		vecfread(poly.Vertexs, hFP);
		vecfread(poly.Indices, hFP);
		vecfread(poly.Meshs, hFP);

		fclose(hFP);
		return ModelData::Create(poly);
		
	}

	HRESULT CreateMaterial(unsigned long count, pmx::t_material* material, std::string* textures, const std::string& sFileName, shared_ptr<MaterialComponent> resultMaterial){
		HRESULT hr = S_OK;

		auto cbm = ConstantBuffer<cbChangesMaterial>::create(4);
		auto cbt = ConstantBuffer<cbChangesUseTexture>::create(6);

		//mpModel->mMeshs.resize(count);
		//UINT vertcount = 0;
		for (DWORD i = 0; i < count; i++){
			Material mat;
			mat.mDiffuse = XMFLOAT4(material[i].material.Diffuse);
			mat.mSpecular = XMFLOAT4(material[i].material.Specular[0], material[i].material.Specular[1], material[i].material.Specular[2], material[i].material.Specularity);
			mat.mAmbient = XMFLOAT4(material[i].material.Ambient[0], material[i].material.Ambient[1], material[i].material.Ambient[2], 1.0f);
			//mpModel->mMeshs[i].mFace_vert_count = material[i].face_vert_count;
			//mpModel->mMeshs[i].mpModelBuffer = this;
			//mpModel->mMeshs[i].mFace_vert_start_count = vertcount;
			//vertcount += material[i].face_vert_count;
			hr = mat.Create(cbm, cbt);
			if (FAILED(hr))
				return hr;
			if (material[i].ToonMode == 1){
				std::string toonFile = "toon";
				toonFile += std::to_string((int)material[i].ToonIdx + 1);
				toonFile += ".bmp";
				toonFile = sFileName + toonFile;

				mat.SetTexture(toonFile.c_str(), 2);
			}
			else{

				if (material[i].ToonIdx != -1){
					std::string file = sFileName + textures[material[i].ToonIdx];
					mat.SetTexture(file.c_str(), 2);
				}
			}

			if (material[i].SphereMode != 0){
				if (material[i].SphereIdx != -1){
					std::string file = sFileName + textures[material[i].SphereIdx];
					mat.SetTexture(file.c_str(), 1);
				}
			}

			if (material[i].TextureIdx != -1){

				std::string file = sFileName + textures[material[i].TextureIdx];
				mat.SetTexture(file.c_str(), 0);
			}

			if (resultMaterial)
				resultMaterial->SetMaterial(i, mat);
		}
		return S_OK;
	}

};