#include "stdafx.h"
#include "Collision.h"
#include "object.h"
#include "Global.h"
#include <math.h>
#include <float.h>

float e = 1.f;	// Åº¼º °è¼ö

bool CollisionCheck(object * oA, object * oB)
{

	float oA_rad, oB_rad;
	float sizeX, sizeY;
	float x1, y1;
	float x2, y2;
	oA->GetSize(&oA_rad, &sizeX);
	oB->GetSize(&oB_rad, &sizeY);
	oA->GetLocation(&x1, &y1);
	oB->GetLocation(&x2, &y2);


	float colLenLimit = oA_rad / 2 + oB_rad / 2;
	float distBtwPoints = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));


	if (colLenLimit <= distBtwPoints)
		return false;

	return true;
}

bool CollisionCheck(object *oA, float posx, float posy)
{

	float oA_rad;
	float Secondsize;
	float x1, y1;
	float x2, y2;
	oA->GetSize(&oA_rad, &Secondsize);
	oA->GetLocation(&x1, &y1);


	float colLenLimit = oA_rad / 2 + PLAYER_SIZE / 2;
	float distBtwPoints = sqrt((posx - x1)*(posx - x1) + (posy - y1)*(posy - y1));


	if (colLenLimit <= distBtwPoints) {
		//printf("¾È°ãÃÄ\n");
		return false;
	}

	//printf("°ãÃÄ\n");
	return true;
}

void CollisionReaction(class object* oA, class object* oB)
{
	int oA_Kind,oB_Kind = 0; 
	oA->GetKind(&oA_Kind);
	oB->GetKind(&oB_Kind);

	
		
		if (oA_Kind == KIND_HERO&&oB_Kind==KIND_BALL)
			{
		
					oA->SetIsVisible(false);
			}
		else if (oA_Kind==KIND_BALL&&oB_Kind == KIND_HERO) {
			
					oB->SetIsVisible(false);
			}
		
	
	
}

void WallCollision(class object* obj)
{
	float posX, posY = 0;
	float rad, height = 0;
	obj->GetLocation(&posX, &posY);
	obj->GetSize(&rad, &height);

	if (posX - rad/2 <= -WINDOW_SIZEX/2||posX+rad/2>= WINDOW_SIZEX / 2) {
		float vx, vy;
		obj->GetPreLocation(&posX, &posY);
		obj->SetLocation(posX, posY);
		obj->GetVelocity(&vx, &vy);
		obj->SetVelocity(-vx*2.f, vy);
	}
	if (posY - rad/2 <= -WINDOW_SIZEY / 2 || posY + rad/2 >= WINDOW_SIZEY / 2) {
		float vx, vy;
		obj->GetPreLocation(&posX, &posY);
		obj->SetLocation(posX, posY);
		obj->GetVelocity(&vx, &vy);
		obj->SetVelocity(vx, -vy*2.f);
	}


}