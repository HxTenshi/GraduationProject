#ifndef _NXCONVEXMESHFOLDER_H_
#define _NXCONVEXMESHFOLDER_H_
#include <NxPhysics.h>
// �ʌ`�󃁃b�V���t�H���_
class NxConvexMeshFolder
{
public:
	// �R���X�g���N�^
	NxConvexMeshFolder(NxPhysicsSDK* physicsSDK, NxU32 numVertices, NxVec3* vertices,
		NxU32 numTriangles = 0, NxU32* indices = 0);
	// �f�X�g���N�^
	~NxConvexMeshFolder();
	// �|�C���^�̎擾
	NxConvexMesh* get();
private:
	// �R�s�[�֎~�i�������j
	NxConvexMeshFolder(const NxConvexMeshFolder& t);
	NxConvexMeshFolder& operator = (const NxConvexMeshFolder& t);
private:
	NxPhysicsSDK* mPhysicsSDK; // PhysicsSDK
	NxConvexMesh* mNxConvexMesh; // �ʌ`�󃁃b�V��
};
#endif