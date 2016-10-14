#pragma once

#include "IComponent.h"
#include "Engine/AssetDataBase.h"

class Material;

class IMaterialComponent :public Component{
public:

	IMaterialComponent(){}
	virtual ~IMaterialComponent(){}


	virtual void SetAlbedoColor(const XMFLOAT4& col) = 0;
	virtual void SetSpecularColor(const XMFLOAT4& col) = 0;
	virtual Material* GetMaterialPtr(UINT GetNo) const =0;
};

class MaterialComponent :public IMaterialComponent{
public:

	void LoadAssetResource(const std::string& path);
	void SaveAssetResource(const std::string& path);

	MaterialComponent();
	~MaterialComponent();

	void Initialize() override;

	void SetMaterial(UINT SetNo, Material& material);
#ifdef _ENGINE_MODE
	void CreateInspector() override;
#endif
	Material* GetMaterialPtr(UINT GetNo) const override;
	Material GetMaterial(UINT GetNo) const;

	void SetAlbedoColor(const XMFLOAT4& col) override;
	void SetSpecularColor(const XMFLOAT4& col) override;

	void IO_Data(I_ioHelper* io) override;

	bool GetUseAlpha() const{
		return mForwardRendering;
	};

	TextureAsset mAlbedoTexture;
	TextureAsset mNormalTexture;
	TextureAsset mHeightTexture;
	TextureAsset mSpecularTexture;
	TextureAsset mRoughnessTexture;
	TextureAsset mEmissiveTexture;
	std::vector<Material> mMaterials;
	std::string mMaterialPath;
	std::string mShaderName;

	XMFLOAT4 mAlbedo;
	XMFLOAT4 mSpecular;
	XMFLOAT2 mTexScale;
	XMFLOAT2 mHeightPower;
	XMFLOAT4 mNormaleScale;
	XMFLOAT2 mOffset;
	float mEmissivePowor;
	bool mForwardRendering;
	float mThickness;
};