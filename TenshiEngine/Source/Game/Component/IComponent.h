#pragma once

#include <String>
#include "Game/Script/GameObject.h"
#include "Game/Parts/Enabled.h"
#include "MySTL/ioHelper.h"


class Actor;
class I_ioHelper;

class Component
	:public Enabled
{
public:
	Component()
		:gameObject(NULL){

	}
	virtual ~Component(){

	}
	//�R���|�[�l���g���ʏ�����
	void _Initialize(GameObject obj){
		gameObject = obj;
	}
	//AddComponent��̏�����
	virtual void Initialize(){

	}
	//GameStart���̏�����
	virtual void Start(){

	}
	//�f�X�g���C���̏���
	virtual void Finish(){

	}
	virtual void Update(){

	}

	virtual void ChangeParentCallback(){

	}

#ifdef _ENGINE_MODE
	virtual void EngineUpdate(){}
	virtual void CreateInspector(){}
#endif
	std::string ClassName(){
		std::string name = typeid(*this).name();
		name.erase(0, 6);//"class "���폜
		return name;
	}
	virtual void IO_Data(I_ioHelper* io) = 0;

	GameObject gameObject;

};