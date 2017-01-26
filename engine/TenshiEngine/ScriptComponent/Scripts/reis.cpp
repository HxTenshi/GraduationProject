#include "reis.h"

#include "UniqueObject.h"

#include "PlayerController.h"

#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

#include "ReisCitrusBullet.h"


struct ReisMode {
	enum Enum {
		Idle,
		Move,
		Attack,
		Attack_Bullet,
		Attack_CitrusBullet,
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
	m_CitrusBulletObject = NULL;
	m_CitrusBulletCount = 0;
	m_CitrusBulletCollDown = 0.0f;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void reis::Start(){

	m_Player = UniqueObject::GetPlayer();
}

//毎フレーム呼ばれます
void reis::Update(){

	m_CitrusBulletCollDown -= Hx::DeltaTime()->GetDeltaTime();

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

			auto r = rand() / (float)RAND_MAX;
			if (r < 0.5f && m_CitrusBulletCollDown<=0.0f) {
				m_ReisMode = ReisMode::Attack_CitrusBullet;
			}else {
				m_ReisMode = ReisMode::Attack_Bullet;
			}
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
	else if (m_ReisMode == ReisMode::Attack_CitrusBullet) {
		Rotate(GetPlayerVect());
		m_CitrusBulletCollDown = 5.0f;
		if (m_CurrentAnimeID != AnimeID::Attack_Bullet) {
			if (m_CitrusBulletObject = Hx::Instance(m_CitrusBullet)) {
				auto pos = gameObject->mTransform->WorldPosition();
				pos.y += 1.0f;
				m_CitrusBulletObject->mTransform->WorldPosition(pos);
				if (auto scr = m_CitrusBulletObject->GetScript<ReisCitrusBullet>()) {
					scr->SetVector(GetPlayerVectH());
				}
			}
		}

		ChangeAnime(AnimeID::Attack_Bullet);
		if (IsCurrentAnimeEnd()) {

			if (m_CitrusBulletObject) {
				auto pos = m_CitrusBulletObject->mTransform->WorldPosition();
				pos.y += 3.0f;
				Teleport(pos);
				Move(XMVectorSet(0.0f , -5.0f, 0.0f, 1.0f));

				Hx::DestroyObject(m_CitrusBulletObject);
				m_CitrusBulletObject = NULL;
			}

			if (m_CitrusBulletCount >= 2) {
				m_CitrusBulletCount = 0;
				m_ReisMode = ReisMode::Idle;
			}
			else {
				ChangeAnime(AnimeID::Idle);
				m_CitrusBulletCount++;
			}
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


void reis::Teleport(const XMVECTOR & vect)
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	cc->Teleport(vect);
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
XMVECTOR reis::GetPlayerVectH()
{
	if (!m_Player)return XMVectorZero();
	auto v = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	v.y = 0.0f;
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
