#include "pch.h"

#include "Collision.h"

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
	int oA_Kind, oB_Kind = 0;
	oA->GetKind(&oA_Kind);
	oB->GetKind(&oB_Kind);

	float oA_X, oA_Y = 0.f;
	float oB_X, oB_Y = 0.f;
	oA->GetLocation(&oA_X, &oA_Y);
	oB->GetLocation(&oB_X, &oB_Y);

	float oA_rad, oB_rad, s1, s2 = 0.f;
	oA->GetSize(&oA_rad, &s1);
	oB->GetSize(&oB_rad, &s2);

	float oA_VX, oA_VY = 0.f;
	oA->GetVelocity(&oA_VX, &oA_VY);

	float oB_VX, oB_VY = 0.f;
	oB->GetVelocity(&oB_VX, &oB_VY);

	float oA_M, oB_M = 0.f;
	oA->GetMass(&oA_M);
	oB->GetMass(&oB_M);

	if (oA_Kind == oB_Kind)
	{
		float dx = oA_X - oB_X;
		float dy = oA_Y - oB_Y;
		float dab = fabsf(sqrt(dx*dx + dy * dy));

		float sinTheta = dy / fabsf(sqrt(dx*dx + dy * dy));
		float cosTheta = dx / fabsf(sqrt(dx*dx + dy * dy));

		float vxAp = (oA_M - e * oB_M) / (oA_M + oB_M)*(oA_VX*cosTheta + oA_VY * sinTheta) +
			(oB_M + e * oB_M) / (oA_M + oB_M)*(oB_VX*cosTheta + oB_VY * sinTheta);
		float vxBp = (oA_M + e * oA_M) / (oA_M + oB_M)*(oA_VX*cosTheta + oA_VY * sinTheta) +
			(oB_M - e * oA_M) / (oA_M + oB_M)*(oB_VX*cosTheta + oB_VY * sinTheta);

		float vyAp = oA_VX * (-sinTheta) + oA_VY * cosTheta;
		float vyBp = oB_VX * (-sinTheta) + oB_VY * cosTheta;

		oA->SetVelocity(vxAp, vyAp);
		oB->SetVelocity(vyAp, vyBp);

	}
	else
	{
		if (oA_Kind == KIND_HERO)
		{
			float oB_VelMag = sqrt(oB_VX*oB_VX + oB_VY * oB_VY);
			if (oB_VelMag > FLT_EPSILON)
				oA->SetIsVisible(false);
		}
		else if (oB_Kind == KIND_HERO) {
			float oA_VelMag = sqrt(oA_VX*oA_VX + oA_VY * oA_VY);
			if (oA_VelMag > FLT_EPSILON)
				oB->SetIsVisible(false);
		}
	}


}

void WallCollision(class object* obj)
{
	float posX, posY = 0;
	float rad, height = 0;
	obj->GetLocation(&posX, &posY);
	obj->GetSize(&rad, &height);

	if (posX - rad / 2 <= -WINDOW_SIZEX / 2 || posX + rad / 2 >= WINDOW_SIZEX / 2) {
		float vx, vy;
		obj->GetPreLocation(&posX, &posY);
		obj->SetLocation(posX, posY);
		obj->GetVelocity(&vx, &vy);
		obj->SetVelocity(-vx * 2.f, vy);
	}
	if (posY - rad / 2 <= -WINDOW_SIZEY / 2 || posY + rad / 2 >= WINDOW_SIZEY / 2) {
		float vx, vy;
		obj->GetPreLocation(&posX, &posY);
		obj->SetLocation(posX, posY);
		obj->GetVelocity(&vx, &vy);
		obj->SetVelocity(vx, -vy * 2.f);
	}


}

bool JoinCollision(class object* obj, float posx, float posy) {
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		if (obj[i].GetIsVisible() == true) {
			if (CollisionCheck(&obj[i], posx, posy))
				return true;
		}
	}

	//printf("¾È°ãÃÄ\n");
	return false;
}