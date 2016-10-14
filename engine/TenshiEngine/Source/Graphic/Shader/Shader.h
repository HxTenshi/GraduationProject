#pragma once

#include "PixelShader.h"
#include "VertexShader.h"
#include "MySTL/Ptr.h"

#include "Engine/AssetDataBase.h"
#include "Engine/Assets.h"

class IShader{
public:
	virtual ~IShader(){};
};

class Shader :public IShader{
public:
	Shader(){

	}
	~Shader(){

	}

	void Create(const char* fileName);
	void Create(ShaderAsset& asset);

	void SetShader(bool UseAnime, ID3D11DeviceContext* context) const;
	void SetShader_VS(bool UseAnime, ID3D11DeviceContext* context) const;
	void SetShader_PS(ID3D11DeviceContext* context) const;

private:
	ShaderAssetDataPtr m_ShaderAssetData;
};