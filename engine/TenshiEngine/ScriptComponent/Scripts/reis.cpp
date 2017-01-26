#include "reis.h"

#include "UniqueObject.h"

#include "PlayerController.h"

#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

struct ReisMode {
	enum Enum {
		Idle,
		Move,
		Attack,
		Attack_Bullet,
	};
};

struct AnimeID {
	enum Enum {
		Idle,
		Move,
		Attack,
		Attack_Bullet,
	};
};


//生成時に呼ばれます（エディター中も呼ばれます）
void reis::Initialize(){
	m_Player = NULL;
	m_ReisMode = ReisMode::Idle;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void reis::Start(){

	m_Player = UniqueObject::GetPlayer();
}

//毎フレーム呼ばれます
void reis::Update(){

	if (m_ReisMode == ReisMode::Idle) {
		ChangeAnime(AnimeID::Idle);
		m_ReisMode = ReisMode::Move;

	}else if (m_ReisMode == ReisMode::Move) {
		ChangeAnime(AnimeID::Move);
		auto v = GetPlayerVect() * m_MoveSpeed * Hx::DeltaTime()->GetDeltaTime();

		Move(v);
		Rotate(v);

		if (GetPlayerLen() < 2.0f) {
			m_ReisMode = ReisMode::Attack;
		}

		if (GetPlayerLen() > 10.0f) {
			m_ReisMode = ReisMode::Attack_Bullet;
		}

	}else if (m_ReisMode == ReisMode::Attack) {
		ChangeAnime(AnimeID::Attack);
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
		}
	}
	else if (m_ReisMode == ReisMode::Attack_Bullet) {
		Rotate(GetPlayerVect());
		if (m_CurrentAnimeID != AnimeID::Attack_Bullet) {
			if (auto obj = Hx::Instance(m_Bullets)) {
				obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());
			}
		}

		ChangeAnime(AnimeID::Attack_Bullet);
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
		}
	}
}


//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void reis::Finish(){

}

//コライダーとのヒット時に呼ばれます
void reis::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void reis::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void reis::OnCollideExit(GameObject target){
	(void)target;
}


void reis::Move(const XMVECTOR & vect)
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	cc->Move(vect);
}

void reis::Rotate(const XMVECTOR & vect)
{
	float m_RotateLimit = 360.0f*3;
	if (XMVector3Length(vect).x == 0)return;
	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 1), vect, XMVectorSet(0, 1, 0, 1)));
	auto q = XMQuaternionRotationMatrix(rotate);

	auto f = gameObject->mTransform->Forward();
	float angle = acos(XMVector3Dot(f, vect).x);
	float limit = m_RotateLimit / 180.0f * XM_PI * Hx::DeltaTime()->GetDeltaTime();
	float t = 1.0f;
	if (limit < angle) {
		t = limit / angle;
	}
	auto wq = gameObject->mTransform->WorldQuaternion();
	q = XMQuaternionSlerp(wq, q, t);
	gameObject->mTransform->WorldQuaternion(q);
}

XMVECTOR reis::GetPlayerVect()
{
	if (!m_Player)return XMVectorZero();
	auto v = m_Player->mTransform->WorldPosition()- gameObject->mTransform->WorldPosition();
	if (XMVector3Length(v).x == 0.0f)return XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	return XMVector3Normalize(v);
}

float reis::GetPlayerLen()
{
	if (!m_Player)return 0.0f;
	return XMVector3Length(m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x;
}

float reis::GetPlayerLenH()
{
	if (!m_Player)return 0.0f;
	auto v1 = m_Player->mTransform->WorldPosition();
	auto v2 = gameObject->mTransform->WorldPosition();
	v1.y = 0.0f;
	v2.y = 0.0f;
	return XMVector3Length(v1 - v2).x;
}

float reis::GetPlayerLenV()
{
	if (!m_Player)return 0.0f;
	return abs(m_Player->mTransform->WorldPosition().y - gameObject->mTransform->WorldPosition().y);
}

void reis::ChangeAnime(int id)
{

	if (m_CurrentAnimeID == id) {
		return;
	}

	if (!m_AnimeModel)return;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return;

	if (m_CurrentAnimeID >= 0) {
		auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
		p.mWeight = 0.0f;
		p.mTime = 0.0f;
		p.mTimeScale = 0.0f;
		anime->SetAnimetionParam(m_CurrentAnimeID, p);
	}
	m_CurrentAnimeID = id;
	auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
	p.mTime = 0.0f;
	p.mWeight = 1.0f;
	p.mTimeScale = 1.0f;
	anime->SetAnimetionParam(m_CurrentAnimeID, p);
}

bool reis::IsCurrentAnimeEnd()
{
	if (!m_AnimeModel)return false;
	if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
		if (anime->IsAnimationEnd(m_CurrentAnimeID)) {
			return true;
		}
	}
	return false;
}
