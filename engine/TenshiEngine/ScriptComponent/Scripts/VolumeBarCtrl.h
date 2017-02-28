
#pragma once
#include "main.h"

#define VOLUME_RATE 100

class VolumeBarCtrl :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void VolumeUp();
	void VolumeDown();
	void SetVolume(int volume);
	int GetVolume();
	bool IsVolumeMax();
	bool IsVolumeMin();
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	GameObject m_memoriBar;
	SERIALIZE
	int m_volume;
	float m_widthScale;
};