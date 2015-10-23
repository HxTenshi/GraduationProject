
#include <NxCooking.h>
#pragma comment( lib, "NxCooking.lib" )

#include "NxConvexMeshFolder.h"
#include "NxMemoryStream.h"

// �R���X�g���N�^
NxConvexMeshFolder::NxConvexMeshFolder(NxPhysicsSDK* physicsSDK, NxU32 numVertices, NxVec3* vertices,
	NxU32 numTriangles, NxU32* indices) :
	mPhysicsSDK(physicsSDK)
{
	// �ʌ`�󃁃b�V���f�[�^�̃p�����[�^��ݒ�
	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices = numVertices; // ���_�f�[�^��
	convexDesc.pointStrideBytes = sizeof(NxVec3); // ���_�f�[�^�̃T�C�Y
	convexDesc.points = vertices; // ���_�f�[�^
	convexDesc.numTriangles = numTriangles; // �O�p�`�f�[�^��
	convexDesc.triangles = indices; // �O�p�`�̒��_�C���f�b�N�X
	convexDesc.triangleStrideBytes = 3 * sizeof(NxU32); // �O�p�`�̒��_�C���f�b�N�X�̃T�C�Y
	convexDesc.flags = NX_CF_COMPUTE_CONVEX; // �ʌ`��̌v�Z���s��
	// �ʌ`�󃁃b�V���f�[�^�쐬������
	NxInitCooking();
	// �������o�b�t�@���쐬
	NxMemoryStream buffer;
	// �ʌ`�󃁃b�V���f�[�^�̍쐬
	NxCookConvexMesh(convexDesc, buffer);
	// �ʌ`�󃁃b�V�����쐬����
	mNxConvexMesh = mPhysicsSDK->createConvexMesh(buffer);
}
// �f�X�g���N�^
NxConvexMeshFolder::~NxConvexMeshFolder()
{
	//mPhysicsSDK->releaseConvexMesh(*mNxConvexMesh);
}
// �|�C���^�̎擾
NxConvexMesh* NxConvexMeshFolder::get()
{
	return mNxConvexMesh;
}