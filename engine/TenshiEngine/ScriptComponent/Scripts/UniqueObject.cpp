#include "UniqueObject.h"

static UniqueObject* g_UniqueObject = NULL;

UniqueObject::UniqueObject()
{
	g_UniqueObject = this;
}

void UniqueObject::Initialize()
{
	g_UniqueObject = this;
}

void UniqueObject::Finish()
{
	g_UniqueObject = NULL;
}

GameObject UniqueObject::GetPlayer()
{
	return g_UniqueObject->m_Player;
}

GameObject UniqueObject::GetCamera()
{
	return g_UniqueObject->m_Camera;
}
