#include "NxMemoryStream.h"
// �P�o�C�g�f�[�^�̓ǂݍ���
NxU8 NxMemoryStream::readByte() const
{
	NxU8 buffer;
	readBuffer(&buffer, sizeof(buffer));
	return buffer;
}
// �Q�o�C�g�f�[�^�̓ǂݍ���
NxU16 NxMemoryStream::readWord() const
{
	NxU16 buffer;
	readBuffer(&buffer, sizeof(buffer));
	return buffer;
}
// �S�o�C�g�f�[�^�̓ǂݍ���
NxU32 NxMemoryStream::readDword() const
{
	NxU32 buffer;
	readBuffer(&buffer, sizeof(buffer));
	return buffer;
}
// �����������f�[�^�̓ǂݍ���
float NxMemoryStream::readFloat() const
{
	float buffer;
	readBuffer(&buffer, sizeof(buffer));
	return buffer;
}
// �������������o�C�g�f�[�^�̓ǂݍ���
double NxMemoryStream::readDouble() const
{
	double buffer;
	readBuffer(&buffer, sizeof(buffer));
	return buffer;
}
// �o�b�t�@�f�[�^�̓ǂݍ���
void NxMemoryStream::readBuffer(void* buffer, NxU32 size) const
{
	for (NxU32 i = 0; i < size; i++) {
		static_cast<NxU8*>(buffer)[i] = mBuffer.front(); // �L���[�̐擪����f�[�^�����o��
		mBuffer.pop(); // �L���[�̐擪���폜
	}
}
// �P�o�C�g�f�[�^�̏�������
NxStream& NxMemoryStream::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(b));
	return *this;
}
// �Q�o�C�g�f�[�^�̏�������
NxStream& NxMemoryStream::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(w));
	return *this;
}
// �S�o�C�g�f�[�^�̏�������
NxStream& NxMemoryStream::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(d));
	return *this;
}
// �����������f�[�^�̏�������
NxStream& NxMemoryStream::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(f));
	return *this;
}
// �������������o�C�g�f�[�^�̏�������
NxStream& NxMemoryStream::storeDouble(NxF64 df)
{
	storeBuffer(&df, sizeof(df));
	return *this;
}
// �o�b�t�@�f�[�^�̏�������
NxStream& NxMemoryStream::storeBuffer(const void* buffer, NxU32 size)
{
	for (NxU32 i = 0; i < size; i++) {
		mBuffer.push(static_cast<const NxU8*>(buffer)[i]); // �L���[�̖����Ƀf�[�^��ǉ�
	}
	return *this;
}