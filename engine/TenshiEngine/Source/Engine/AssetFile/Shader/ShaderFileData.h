#pragma once

#include <string>
#include "../AssetFileData.h"

#include "Graphic/Shader/PixelShader.h"
#include "Graphic/Shader/VertexShader.h"
#include "MySTL/Ptr.h"
#include "Window/Window.h"

class ShaderData{
public:
	ShaderData(){

	}
	~ShaderData(){

	}

	void Create(const char* fileName){
		if (mVertexShader) {
			mVertexShader->Release();
		}
		if (mVertexShaderAnime) {
			mVertexShaderAnime->Release();
		}
		if (mPixelShader) {
			mPixelShader->Release();
		}
		if (!mVertexShader) {
			mVertexShader = make_shared<VertexShader>();
		}
		if (!mVertexShaderAnime) {
			mVertexShaderAnime = make_shared<VertexShader>();
		}
		if (!mPixelShader) {
			mPixelShader = make_shared<PixelShader>();
		}

		if (FAILED(mVertexShader->Create(fileName, "VS"))){
			mVertexShader->Create("EngineResource/ErrorShader.fx", "VS");
			Window::AddLog(std::string(fileName)+":VS作成失敗");
			_SYSTEM_LOG_ERROR("シェーダー[" + fileName + ":VS]の作成");
		}
		
		if (FAILED(mVertexShaderAnime->Create(fileName, "VSSkin"))){
			mVertexShaderAnime->Create("EngineResource/ErrorShader.fx","VS");
			Window::AddLog(std::string(fileName) + ":VSSkin作成失敗");
			_SYSTEM_LOG_ERROR("シェーダー[" + fileName + ":VSSkin]の作成");
		}
		if (FAILED(mPixelShader->Create(fileName))){
			mPixelShader->Create("EngineResource/ErrorShader.fx", "PS");
			Window::AddLog(std::string(fileName) + ":PS作成失敗");
			_SYSTEM_LOG_ERROR("シェーダー[" + fileName + ":PS]の作成");
		};
	}

	void SetShader(bool UseAnime, ID3D11DeviceContext* context) const{
		if (UseAnime){
			if (mVertexShaderAnime)
				mVertexShaderAnime->SetShader(context);
		}
		else{
			if (mVertexShader)
				mVertexShader->SetShader(context);
		}
		if (mPixelShader)
			mPixelShader->SetShader(context);
	}
	void SetShader_VS(bool UseAnime, ID3D11DeviceContext* context) const{
		if (UseAnime){
			if (mVertexShaderAnime)
				mVertexShaderAnime->SetShader(context);
		}
		else{
			if (mVertexShader)
				mVertexShader->SetShader(context);
		}
	}
	void SetShader_PS(ID3D11DeviceContext* context) const{

		if (mPixelShader)
			mPixelShader->SetShader(context);
	}

	shared_ptr<PixelShader> mPixelShader;
	shared_ptr<VertexShader> mVertexShader;
	shared_ptr<VertexShader> mVertexShaderAnime;
};

class ShaderFileData : public AssetFileData{
public:
	ShaderFileData();
	~ShaderFileData();

	bool Create(const char* filename) override;

	void Compile();

	const ShaderData* GetShader() const{
		return m_Shader;
	}


private:
	//コピー禁止
	ShaderFileData(const ShaderFileData&) = delete;
	ShaderFileData& operator=(const ShaderFileData&) = delete;

	ShaderData* m_Shader;
};
