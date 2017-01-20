#include "UniqueObject.h"

static UniqueObject* g_UniqueObject = NULL;

void UniqueObject::Initialize()
{
	g_UniqueObject = this;
}

void UniqueObject::Finish()
{
	g_UniqueObject = NULL;
}

bool UniqueObject::IsPlayer(GameObject target)
{
	if (!g_UniqueObject)return false;
	return g_UniqueObject->m_Player == target;
}

GameObject UniqueObject::GetPlayer()
{
	if (!g_UniqueObject)return NULL;
	return g_UniqueObject->m_Player;
}

GameObject UniqueObject::GetCamera()
{
	if (!g_UniqueObject)return NULL;
	return g_UniqueObject->m_Camera;
}
