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
	void RenderScene();
	void Update(float elapsed_time_in_sec);
	void ApplyForce(float forceX, float forceY, float elapsed_time_in_sec);
	void BreakMovement(bool W_KeyIsDown, bool S_KeyIsDown, bool D_KeyIsDown, bool A_KeyIsDown, float elapsed_time_in_sec);
	void AddObject(float px, float py, float pz, float sx, float sy, float vx, float vy, int kind);
	void DeleteObject(unsigned int id);
	void ObjectCollision();
	int FindEmptyObjectSlot();

	void joinClick(int key);
	void RenderJoin();
private:
	object *objs[MAX_OBJECTS];
};

DWORD WINAPI ScnMgrThread(LPVOID arg);
void InitObject();
void ObjectUpdate();
void ObjectCollision();