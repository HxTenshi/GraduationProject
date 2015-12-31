#pragma once

#include <d3d11.h>
#include "XNAMath/xnamath.h"

class Material;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};


struct PMDVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
	UINT BoneWeight[4];
};

// �K�w�\��
class Hierarchy
{
public:
	Hierarchy() : mIdxSelf((UINT)-1), mIdxParent((UINT)-1){}
	UINT mIdxSelf;		// �{�[��idx
	UINT mIdxParent;	// �e�K�widx
};

#include <string>
// �{�[��
class Bone
{
public:
	Bone() : mScale(1, 1, 1), mRot(0, 0, 0, 1), mPos(1, 1, 1){}
	std::string mStrName;
	XMFLOAT3 mScale;	// �X�P�[��xyz
	XMFLOAT4 mRot;		// ��]�i�N�H�[�^�j�I���j
	XMFLOAT3 mPos;		// �ʒu
	XMMATRIX mMtxPose;	//
	XMMATRIX mMtxPoseInit;	//
	WORD mIkBoneIdx;
	Hierarchy mHierarchy;
private:
};

#include <vector>

//IK
struct Ik
{
	UINT bone_index; // IK�{�[���ԍ�
	UINT target_bone_index; // IK�^�[�Q�b�g�{�[���ԍ� // IK�{�[�����ŏ��ɐڑ�����{�[��
	UINT chain_length; // IK�`�F�[���̒���(�q�̐�)
	UINT iterations; // �ċA���Z�� // IK�l1
	float control_weight; // IK�̉e���x // IK�l2 // ���Z1�񂠂���̐����p�x
	std::vector<UINT> child_bone_index;//[ik_chain_length] IK�e�����̃{�[���ԍ�
};



class Model;
#include "../Loader/Animation/VMD.h"

class KeyFrame{
public:
	KeyFrame(const vmd::VMDKeyFrame& key):mKeyFrame(key){}
	vmd::VMDKeyFrame mKeyFrame;
	bool operator<(const KeyFrame& o){ return mKeyFrame.FrameNo < o.mKeyFrame.FrameNo; }
private:
};

class Motion{
public:
	std::vector<KeyFrame> mKeyFrame;
private:
};


#include "MySTL/ptr.h"
class MaterialComponent;
class ModelBuffer{
public:
	ModelBuffer();
	virtual ~ModelBuffer();
	virtual HRESULT Create(const char* FileName, Model* mpModel);
	void IASet() const;
	void Draw(UINT IndexNum, UINT StartIndex) const;
	void Release();

	void VMDMotionCreate(vmd& anime);

	// �Ƃ肠�����A�j���[�V����
	void VMDAnimation(float key_time);
	// �Ƃ肠����IK
	void VMDIkAnimation();

	void UpdatePose();

protected:

	HRESULT createVertex(void* Vertices, UINT TypeSize, UINT VertexNum);
	HRESULT createIndex(WORD* Indices, UINT IndexNum);
protected:
	ID3D11Buffer*	mpVertexBuffer;
	ID3D11Buffer*	mpIndexBuffer;

	UINT mIndexNum;

	DWORD mBoneNum;
	Bone* mBone;
	DWORD mIkNum;
	Ik* mIk;

	Motion* mMotion;

	UINT mStride;

	XMVECTOR mMaxVertex;
	XMVECTOR mMinVertex;

	friend Model;
};


#include "Engine/AssetLoader.h"
class AssetModelBuffer : public ModelBuffer {
public:
	HRESULT Create(const char* FileName, Model* mpModel) override;
private:
	friend Model;
};
