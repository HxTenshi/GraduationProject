#pragma once

#include "Component.h"
#include "Engine/AssetFile/NaviMesh/NaviMeshFileData.h"

class INaviMeshComponent {
public:
	INaviMeshComponent() {}
	virtual ~INaviMeshComponent(){}

	virtual void SetBaseNaviMeshObject(GameObject obj) = 0;
	virtual void SetNaviMesh(MeshAsset mesh) = 0;
	virtual void RootCreate(GameObject start, GameObject end) = 0;
	virtual void Move(float speed) = 0;
	virtual bool IsMoveEnd() = 0;
	virtual XMVECTOR GetPosition() = 0;
	virtual XMVECTOR GetRouteVector() = 0;
	virtual float GetRemainderRouteDistance() = 0;
	virtual float GetRouteDistance() = 0;
};

class NaviMeshComponent : public INaviMeshComponent, public Component {
public:
	NaviMeshComponent();
	~NaviMeshComponent();
	void Initialize() override;
#ifdef _ENGINE_MODE
	void EngineUpdate() override;
#endif
	void Update() override;
	void Finish() override;

#ifdef _ENGINE_MODE
	void CreateInspector() override;
#endif
	void IO_Data(I_ioHelper* io) override;
	//�����ς݂̃i�r���b�V�����Q�Ƃ���ݒ�@
	//�ݒ肷���SetNaviMesh�Ő��������i�r���b�V����������
	void SetBaseNaviMeshObject(GameObject obj) override;
	//MeshAsset����i�r���b�V���𐶐�����
	//�ݒ肷���SetBaseNaviMeshComponent�Őݒ肵���Q�Ƃ��؂��
	void SetNaviMesh(MeshAsset mesh) override;
	void RootCreate(GameObject start, GameObject goal) override;
	//���݂̈ʒu��i�߂�
	void Move(float speed) override;
	bool IsMoveEnd() override;
	//���݂̈ʒu
	XMVECTOR GetPosition()  override;
	//���݂̈ʒu���瓹�̕���
	XMVECTOR GetRouteVector() override;
	//���݂̈ʒu����c��̋���
	float GetRemainderRouteDistance() override;
	//�����[�g����
	float GetRouteDistance() override;

private:

#ifdef _ENGINE_MODE
	bool m_EngineRouteView;
	bool m_EngineMeshView;
	bool m_EngineWallView;
#endif

	MeshAsset m_Mesh;
	NaviMesh mNaviMesh;
	GameObject mBaseNaviMeshObject;
	shared_ptr<NavigateCreator> mNaviMeshCreatorPtr;
	shared_ptr<Navigate> mNavigatePtr;
	NaviMeshPolygon* mStart;
	NaviMeshPolygon* mEnd;

	GameObject mStartTarget;
	GameObject mEndTarget;
};