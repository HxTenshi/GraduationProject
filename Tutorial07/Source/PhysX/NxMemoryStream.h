#ifndef _NXMEMORYSTREAM_H_
#define _NXMEMORYSTREAM_H_
#include <NxPhysics.h>
#include <NxStream.h>
#include <queue>
// �������X�g���[���N���X
class NxMemoryStream : public NxStream
{
public:
	// �P�o�C�g�f�[�^�̓ǂݍ���
	NxU8 readByte() const;
	// �Q�o�C�g�f�[�^�̓ǂݍ���
	NxU16 readWord() const;
	// �S�o�C�g�f�[�^�̓ǂݍ���
	NxU32 readDword() const;
	// �����������f�[�^�̓ǂݍ���
	float readFloat() const;
	// �������������o�C�g�f�[�^�̓ǂݍ���
	double readDouble() const;
	// �o�b�t�@�f�[�^�̓ǂݍ���
	void readBuffer(void* buffer, NxU32 size) const;
	// �P�o�C�g�f�[�^�̏�������
	NxStream& storeByte(NxU8 b);
	// �Q�o�C�g�f�[�^�̏�������
	NxStream& storeWord(NxU16 w);
	// �S�o�C�g�f�[�^�̏�������
	NxStream& storeDword(NxU32 d);
	// �����������f�[�^�̏�������
	NxStream& storeFloat(NxReal f);
	// �������������o�C�g�f�[�^�̏�������
	NxStream& storeDouble(NxF64 df);
	// �o�b�t�@�f�[�^�̏�������
	NxStream& storeBuffer(const void* buffer, NxU32 size);
private:
	// �o�b�t�@�f�[�^
	mutable std::queue<NxU8> mBuffer;
};
#endif