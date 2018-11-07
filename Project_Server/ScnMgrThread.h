#pragma once
#include "object.h"
#include "Server_Global.h"
#include "Server_Handle.h"
#include "Collision.h"
class ScnMgr
{
public:
	ScnMgr();
	~ScnMgr();
	void InitObject();
	void RenderScene();
	void Update(float elapsed_time_in_sec);
	void ObjectCollision();
	int FindEmptyObjectSlot();
private:
	object *objs[MAX_OBJECTS];
};

DWORD WINAPI ScnMgrThread(LPVOID arg);