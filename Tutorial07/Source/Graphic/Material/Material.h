#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Texture.h"
#include "ConstantBuffer/ConstantBuffer.h"
#include "ConstantBuffer/ConstantBufferDif.h"

class Material{
public:
	Material();
	~Material();

	HRESULT Create(ConstantBuffer<cbChangesMaterial> const& cbMaterial, ConstantBuffer<cbChangesUseTexture> const& cbUseTexture);
	void PSSetShaderResources() const;
	void SetTexture(const char* FileName, UINT Slot = 0);
	void SetTexture(const Texture& Tex, UINT Slot = 0);

	void ExportData(File& f);
public:

	XMFLOAT4 mDiffuse;
	XMFLOAT4 mSpecular;
	XMFLOAT4 mAmbient;
	//unsigned char toon_index;
	//unsigned char edge_flag;//	�֊s�t���O

private:
public:
	ConstantBuffer<cbChangesMaterial> mCBMaterial;
	ConstantBuffer<cbChangesUseTexture> mCBUseTexture;
	Texture mTexture[4];
};