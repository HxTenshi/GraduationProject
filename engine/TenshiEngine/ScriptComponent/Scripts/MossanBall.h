
#pragma once
#include "main.h"


class MossanBall :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void ExplodeTimer();
	void ExplodeFunc();
	void SetDir(XMVECTOR dir);
	void Shooter();
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE XMVECTOR direction;
	SERIALIZE float speed;
	SERIALIZE float explodeTime;


	SERIALIZE float instance_time;
	float timer;
	bool enable;

	float count_explode_time;
};