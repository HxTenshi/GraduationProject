
#include "SoundFileData.h"
#include <dsound.h>
#include "Sound/Sound.h"


void SoundFile::Release(){
	//if (mBuffer){
	//	mBuffer->Release();
	//	mBuffer = NULL;
	//}

	//if (m3DBuffer){
	//	m3DBuffer->Release();
	//	m3DBuffer = NULL;
	//}
}

bool SoundFile::Create(const char* filename){
	if (mBuffer) {
		mBuffer->Release();
		mBuffer = NULL;
	}
	if (m3DBuffer) {
		m3DBuffer->Release();
		m3DBuffer = NULL;
	}

	// Wave�t�@�C���I�[�v��
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	if (!openWave((CHAR*)filename, wFmt, &pWaveData, waveSize)){
		return false;
	}

	if (wFmt.nChannels == 2) {
		auto bufferLenght = wFmt.nSamplesPerSec * wFmt.nChannels;

		if (0) {
			int i;
			int monopos = 0;
			for (i = 0; i < bufferLenght; i += 2) {
				//���m������
				short t1 = (short)((((int)pWaveData[i]) +((int)pWaveData[i+1]))/2);

				pWaveData[monopos] = t1;
				monopos++;
			}
		}

		waveSize /= 2;
		wFmt.nChannels = 1;
		wFmt.nBlockAlign /= 2;
		wFmt.nAvgBytesPerSec = wFmt.nSamplesPerSec * wFmt.nBlockAlign;
	}


	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	if (_SOUND3D_MODE){
		DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D;
		//DSBufferDesc.dwFlags = DSBCAPS_CTRL3D;
		//if (wFmt.nChannels != 2) {
		//	DSBufferDesc.dwFlags != DSBCAPS_CTRL3D;
		//}
	DSBufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	}
	else{
		DSBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
		DSBufferDesc.guid3DAlgorithm = GUID_NULL;
	}
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;


	if (_SOUND3D_MODE){
		IDirectSoundBuffer *ptmpBuf = 0;
		SoundDevice::GetDevice()->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
		ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&mBuffer);
		ptmpBuf->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&m3DBuffer);
		ptmpBuf->Release();
		if (mBuffer == 0) {
			return false;
		}
	}
	else{
		IDirectSoundBuffer *ptmpBuf = 0;
		SoundDevice::GetDevice()->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
		ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&mBuffer);
		ptmpBuf->Release();
		if (mBuffer == 0) {
			return false;
		}
	}

	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == mBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pWaveData, dwLength);
		mBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	delete[] pWaveData; // �����͂�������Ȃ�
	return true;
}

// Wave�t�@�C���I�[�v���֐�
bool SoundFile::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
	if (filepath == 0)
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Wave�t�@�C���I�[�v��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return false; // �t�@�C���I�[�v�����s

	// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	*ppData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[] * ppData;
		return false;
	}
	dataSize = size;

	// �n���h���N���[�Y
	mmioClose(hMmio, 0);

	return true;
}

//// Wave�t�@�C���I�[�v���֐�
//bool SoundFile::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
//	if (filepath == 0)
//		return false;
//
//	HMMIO hMmio = NULL;
//	MMIOINFO mmioInfo;
//
//	// Wave�t�@�C���I�[�v��
//	memset(&mmioInfo, 0, sizeof(MMIOINFO));
//	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
//	if (!hMmio)
//		return false; // �t�@�C���I�[�v�����s
//
//					  // RIFF�`�����N����
//	MMRESULT mmRes;
//	MMCKINFO riffChunk;
//	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
//	if (mmRes != MMSYSERR_NOERROR) {
//		mmioClose(hMmio, 0);
//		return false;
//	}
//
//	// �t�H�[�}�b�g�`�����N����
//	MMCKINFO formatChunk;
//	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
//	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
//	if (mmRes != MMSYSERR_NOERROR) {
//		mmioClose(hMmio, 0);
//		return false;
//	}
//	DWORD fmsize = formatChunk.cksize;
//	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
//	if (size != fmsize) {
//		mmioClose(hMmio, 0);
//		return false;
//	}
//
//	mmioAscend(hMmio, &formatChunk, 0);
//
//	// �f�[�^�`�����N����
//	MMCKINFO dataChunk;
//	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
//	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
//	if (mmRes != MMSYSERR_NOERROR) {
//		mmioClose(hMmio, 0);
//		return false;
//	}
//	*ppData = new char[dataChunk.cksize];
//	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
//	if (size != dataChunk.cksize) {
//		delete[] * ppData;
//		return false;
//	}
//	dataSize = size;
//
//
//	if (0) {
//		int i;
//		for (i = 0; i<bufferLenght; i += 2) {
//			//���m������
//			//short t1 = (short)((((int)buf[i]) +((int)buf[i+1]))/2);
//
//			//�^�񒆂̉�������
//			short t1 = (short)(((int)buf[i]) - ((int)buf[i + 1]));
//
//			buf[i] = buf[i + 1] = t1;
//		}
//	}
//
//
//
//	// �n���h���N���[�Y
//	mmioClose(hMmio, 0);
//
//	return true;
//}

void SoundFile::Volume(float vol)const{
	//DSBVOLUME_MAX (�����Ȃ�) ���� DSBVOLUME_MIN (����)
	LONG v = (LONG)(DSBVOLUME_MIN - DSBVOLUME_MIN*vol);
	//�ύX����Active����Ȃ��Ƃ����Ȃ��H
	mBuffer->SetVolume(v);
}
void SoundFile::Play(bool loop,bool resetPlayPoint)const{
	if (!mBuffer)return;
	if (resetPlayPoint)mBuffer->SetCurrentPosition(0);
	if (loop){
		mBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}
	else{
		mBuffer->Play(0, 0, NULL);
	}
}

void SoundFile::Duplicate(const SoundFile* out)const{
	if (!out)return;
	if (out->mBuffer)return;
	SoundDevice::GetDevice()->DuplicateSoundBuffer(mBuffer, (LPDIRECTSOUNDBUFFER*)&out->mBuffer);

	if (_SOUND3D_MODE){
		out->mBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&out->m3DBuffer);
	}
}



void SoundFile::Stop()const{
	if (mBuffer)mBuffer->Stop();
}

bool SoundFile::IsPlay()const{
	if (!mBuffer)return false;
	DWORD status = 0;
	mBuffer->GetStatus(&status);
	return (status & DSBSTATUS_PLAYING) != 0;
}

void SoundFile::Set3DPosition(float x, float y, float z) const{
	if (!m3DBuffer)return;
	auto hr = m3DBuffer->SetPosition(x, y, z, DS3D_IMMEDIATE);
}
void SoundFile::Set3DMode(bool flag) const {
	if (!m3DBuffer)return;
	if (flag) {
		m3DBuffer->SetMode(DS3DMODE_NORMAL, DS3D_IMMEDIATE);
	}
	else {
		m3DBuffer->SetMode(DS3DMODE_DISABLE, DS3D_IMMEDIATE);
	}


}

SoundFileData::SoundFileData(){
}
SoundFileData::~SoundFileData(){
}

bool SoundFileData::Create(const char* filename){
	m_FileName = filename;
	return m_SoundData.Create(filename);
}