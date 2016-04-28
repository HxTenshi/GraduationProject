#pragma once

#include <string>
#include "../AssetFileData.h"

#include "Graphic/Shader/PixelShader.h"
#include "Graphic/Shader/VertexShader.h"
#include "MySTL/Ptr.h"

class ShaderData{
public:
	ShaderData(){

	}
	~ShaderData(){

	}

	void Create(const char* fileName){
		mVertexShader = make_shared<VertexShader>();
		mVertexShaderAnime = make_shared<VertexShader>();
		mPixelShader = make_shared<PixelShader>();

		if (FAILED(mVertexShader->Create(fileName, "VS"))){
			//MessageBox(NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file. VS", "Error", MB_OK);
		}
		mVertexShaderAnime->Create(fileName, "VSSkin");
		mPixelShader->Create(fileName);
	}

	void SetShader(bool UseAnime) const{
		if (UseAnime){
			if (mVertexShaderAnime)
				mVertexShaderAnime->SetShader();
		}
		else{
			if (mVertexShader)
				mVertexShader->SetShader();
		}
		if (mPixelShader)
			mPixelShader->SetShader();
	}

	shared_ptr<PixelShader> mPixelShader;
	shared_ptr<VertexShader> mVertexShader;
	shared_ptr<VertexShader> mVertexShaderAnime;
};

class ShaderFileData : public AssetFileData{
public:
	ShaderFileData();
	~ShaderFileData();

	void Create(const char* filename) override;
	void FileUpdate() override;

	void Compile();

	const ShaderData* GetShader() const{
		return m_Shader;
	}


private:
	//�R�s�[�֎~
	ShaderFileData(const ShaderFileData&) = delete;
	ShaderFileData& operator=(const ShaderFileData&) = delete;

	ShaderData* m_Shader;
};
