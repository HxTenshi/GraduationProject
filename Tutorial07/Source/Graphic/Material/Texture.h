#pragma once

#define _TEXTURE_LOADTYPE_DIRECTXTEX_ 0

#include <D3D11.h>
#include "MySTL/ptr.h"
#include <string>

class File;

class TextureData{
public:
	TextureData()
		:mpTextureRV(NULL)
		, mpSamplerLinear(NULL)
	{
	}
	~TextureData(){
		Release();
	}
	void Release(){
		if (mpTextureRV){
			mpTextureRV->Release();
			mpTextureRV = NULL;
		}
		if (mpSamplerLinear){
			mpSamplerLinear->Release();
			mpSamplerLinear = NULL;
		}
	}

	ID3D11ShaderResourceView*	mpTextureRV;
	ID3D11SamplerState*			mpSamplerLinear;
};

class Texture{
public:
	Texture();
	~Texture();
	HRESULT Create(ID3D11ShaderResourceView* pTexture);
	HRESULT Create(const char* FileName);
	void PSSetShaderResources(UINT Slot) const;

#if _TEXTURE_LOADTYPE_DIRECTXTEX_
	HRESULT LoadDirectXTex();
#define LoadTexture LoadDirectXTex

#else
	HRESULT LoadD3DX11();
#define LoadTexture LoadD3DX11

#endif

	void ExportData(File& f);

private:
	shared_ptr<TextureData> mTexturePtr;

	std::string mFileName;
	static Texture mNullTexture;
};