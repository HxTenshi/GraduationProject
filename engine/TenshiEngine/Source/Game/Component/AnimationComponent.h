#pragma once

#include "IComponent.h"
#include "MySTL/ptr.h"

#include "Graphic/Model/AnimeData.h"
#include "Graphic/Model/AnimationBind.h"
#include "Engine/Assets.h"

struct AnimeParam{
	//�A�j���[�V�����̍Đ��ʒu
	float mTime = 0.0f;
	//�A�j���[�V�����̍Đ����x
	float mTimeScale = 1.0f;
	//�A�j���[�V�����̃��[�v
	bool mLoop = true;
	//�A�j���[�V�����̓K�p��
	float mWeight = 0.0f;

	//�A�j���[�V����VMD�t�@�C��
	AnimationAsset mAnimationAsset;
};

class BoneModel;
struct AnimeSet{
	AnimeSet();
	~AnimeSet();
	void Create();

	void Bind(BoneModel* bone);

	void Update();

	AnimeParam Param;

	shared_ptr<AnimationBind> mAnimationBind;
};

class IAnimationComponent{
public:
	virtual ~IAnimationComponent(){}
	virtual AnimeParam GetAnimetionParam(int id) = 0;
	virtual void SetAnimetionParam(int id, const AnimeParam& param) = 0;
	virtual float GetTotalTime(int id) = 0;
};

class ModelComponent;
//class vmd;
class AnimationComponent :public IAnimationComponent, public Component{
public:
	AnimationComponent();
	void Initialize() override;
	void Start() override;
	void Finish() override;
	void Update() override;
#ifdef _ENGINE_MODE
	void CreateInspector() override;
#endif

	AnimeParam GetAnimetionParam(int id) override;
	void SetAnimetionParam(int id,const AnimeParam& param) override;
	float GetTotalTime(int id) override;

	void IO_Data(I_ioHelper* io) override;
private:
	void changeAnimetion(int id);
	weak_ptr<ModelComponent> mModel;

	int mCurrentSet;
	AnimeSet mView;
	std::vector<AnimeSet> mAnimationSets;
	int mAnimetionCapacity;
};