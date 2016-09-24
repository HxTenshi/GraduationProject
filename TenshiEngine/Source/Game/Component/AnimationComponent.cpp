
#include <functional>
#include "AnimationComponent.h"

#include "ModelComponent.h"

#include "Component.h"
#include "Graphic/Loader/Animation/VMD.h"
#include "Graphic/Model/Model.h"
#include "Graphic/Model/BoneModel.h"


#include "Engine/Inspector.h"

AnimationComponent::AnimationComponent()
	:mCurrentSet(0)
	, mAnimetionCapacity(10)
	, mAnimationSets(10)
{}
void AnimationComponent::Initialize(){
	for (auto& set : mAnimationSets){
		set.Create();
	}
	ChangeAnimetion(0);
}

void AnimationComponent::Finish(){
	mAnimationSets.clear();
}
void AnimationComponent::Update(){


	if (!mModel){
		mModel = gameObject->GetComponent<ModelComponent>();
		if (!mModel)return;
	}
	if (mModel && mModel->mModel && mModel->mModel->mBoneModel){

		auto bone = mModel->mModel->mBoneModel;

		for (auto& set : mAnimationSets){
			set.Update(bone);
		}

		auto& set = mAnimationSets[mCurrentSet];
		mView.Param = set.Param;

		std::vector<shared_ptr<AnimationBind>> binds(mAnimetionCapacity);
		
		for (int i = 0; i < mAnimetionCapacity; i++){
			binds[i] = mAnimationSets[i].mAnimationBind;
		}

		bone->UpdateAnimation(binds);
	}
}
#ifdef _ENGINE_MODE
void AnimationComponent::CreateInspector(){

	Inspector ins("Animetion",this);
	ins.AddEnableButton(this);

	std::function<void(int)> collbackset = [&](int f){
		ChangeAnimetion(f);
	};
	ins.Add("ID", &mCurrentSet, collbackset);

	std::function<void(float)> collback = [&](float f){
		mView.Param.mTime = f;
		mAnimationSets[mCurrentSet].Param.mTime = f;
	};
	ins.Add("Time", &mView.Param.mTime, collback);


	std::function<void(float)> collbackscale = [&](float f){
		mView.Param.mTimeScale = f;
		mAnimationSets[mCurrentSet].Param.mTimeScale = f;
	};
	ins.Add("TimeScale", &mView.Param.mTimeScale, collbackscale);

	std::function<void(float)> collbackw = [&](float f){
		mView.Param.mWeight = f;
		mAnimationSets[mCurrentSet].Param.mWeight = f;
		if (mAnimationSets[mCurrentSet].mAnimationBind){
			mAnimationSets[mCurrentSet].mAnimationBind->SetWeight(f);
		}
	};
	ins.Add("Weight", &mView.Param.mWeight, collbackw);

	std::function<void(bool)> collbackloop= [&](bool f){
		mView.Param.mLoop = f;
		mAnimationSets[mCurrentSet].Param.mLoop = f;
		if (mAnimationSets[mCurrentSet].mAnimationBind){
			 mAnimationSets[mCurrentSet].mAnimationBind->SetLoopFlag(f);
		}
	};
	ins.Add("Loop", &mView.Param.mLoop, collbackloop);

	std::function<void(std::string)> collbackpath = [&](std::string name){
		mView.Param.mFileName = name;
		mAnimationSets[mCurrentSet].Param.mFileName = name;
		mAnimationSets[mCurrentSet].Create();

	};
	ins.Add("VMD", &mView.Param.mFileName, collbackpath);

	ins.Complete();
}
#endif

void AnimationComponent::IO_Data(I_ioHelper* io){

	Enabled::IO_Data(io);

#define _KEY(i_,x) io->func( x , (#x + std::to_string(##i_)).c_str() )

	for (int i = 0; i < mAnimetionCapacity; i++){
		_KEY(i, mAnimationSets[i].Param.mFileName);
		_KEY(i, mAnimationSets[i].Param.mTime);
		_KEY(i, mAnimationSets[i].Param.mTimeScale);
		_KEY(i, mAnimationSets[i].Param.mLoop);
		_KEY(i, mAnimationSets[i].Param.mWeight);
	}

#undef _KEY
}

void AnimationComponent::ChangeAnimetion(int id){
	mCurrentSet = min(max(id, 0), (mAnimetionCapacity - 1));
	auto& set = mAnimationSets[mCurrentSet];
	mView.Param = set.Param;
}

AnimeParam AnimationComponent::GetAnimetionParam(int id){
	id = min(max(id, 0), (mAnimetionCapacity - 1));
	return mAnimationSets[id].Param;
}
void AnimationComponent::SetAnimetionParam(int id,const AnimeParam& param){
	id = min(max(id, 0), (mAnimetionCapacity - 1));
	bool createflag = false;
	if (mAnimationSets[id].Param.mFileName != param.mFileName){
		createflag = true;
	}

	mAnimationSets[id].Param = param;
	if (createflag){
		mAnimationSets[id].Create();

	}

	mAnimationSets[id].mAnimationBind->SetWeight(mAnimationSets[id].Param.mWeight);
	mAnimationSets[id].mAnimationBind->SetLoopFlag(mAnimationSets[id].Param.mLoop);
	mAnimationSets[id].mAnimationBind->PlayAnimetionSetTime(mAnimationSets[id].Param.mTime);
}





#include "Game/Game.h"

void AnimeSet::Update(BoneModel* bone){
	//なければ作成
	if (!mAnimationBind){
		mAnimationBind = bone->BindAnimation(&mAnimeData);

		mAnimationBind->SetLoopFlag(Param.mLoop);
		mAnimationBind->SetWeight(Param.mWeight);
		mAnimationBind->PlayAnimetionSetTime(Param.mTime);
	}

	if (!mAnimationBind)return;
	auto time = Game::GetDeltaTime()->GetDeltaTime();
	Param.mTime = mAnimationBind->GetTime();
	//１秒で３０フレーム
	mAnimationBind->PlayAnimetionAddTime((30)*time * Param.mTimeScale);
}