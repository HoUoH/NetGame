#pragma once

#include "Renderer.h"
#include "object.h"
#include "Global.h"
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
	/*int FindEmptyObjectSlot();*/
	//게임 새로 시작 및 시작
	void joinClick(char key);
	void RenderJoin();
	void SetMyID(int i);
	void UpdateRecvData(float posx, float posy, bool isvisible, int i);
	void getSendData(float * posX, float * posY, bool * isVisible);
private:
	Renderer *m_Renderer;
	object *objs[MAX_OBJECTS];
	GLuint Character_Texture[8];
	GLuint Ball_Texture;
	GLuint Join_Texture;
	GLuint Background_Texture;
	GLuint Txt_Texture;
	GLuint base;
	int MyID;
	float GameTime;
	float Invincible_time;
	float temp;
	float Invincible_limit;
};

