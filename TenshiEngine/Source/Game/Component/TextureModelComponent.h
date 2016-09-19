#pragma once

#include "IModelComponent.h"

#include "Engine/AssetDataBase.h"

class Model;
class Material;

class ITextureModel{
public:
	virtual ~ITextureModel(){}

	virtual void SetTexture(TextureAsset& asset) = 0;
};

class TextureModelComponent :public ITextureModel, public IModelComponent{
public:
	TextureModelComponent();
	~TextureModelComponent();

	void Initialize() override;
	void Start() override;
	void EngineUpdate() override;
	void Update() override;
	void Finish() override;

	void SetMatrix() override;

#ifdef _ENGINE_MODE
	void CreateInspector() override;
#endif

	void IO_Data(I_ioHelper* io) override;

	void SetTexture(TextureAsset& asset) override;
private:
	Material* mMaterial;
	TextureAsset m_TextureAsset;
};