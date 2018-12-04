#include "pch.h"
#include "Collision.h"
#include "Server_Global.h"

float e = 1.f;	// 탄성 계수


//object vs postion ver
bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y, float posx, float posy)
{

	float colLenLimit = oA_rad / 2 + PLAYER_SIZE / 2;
	float distBtwPoints = sqrt((posx - oA_x)*(posx - oA_x) + (posy - oA_y)*(posy - oA_y));


	if (colLenLimit - distBtwPoints < FLT_EPSILON) {		// colLenLimit - distBtwPoints < FLT_EPSILON
		//printf("안겹쳐\n");
		return false;
	}

	//printf("겹쳐\n");
	return true;
}

//object vs object ver
bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y, 
	const float& oB_rad, const float& oB_x, const float& oB_y)
{

	float colLenLimit = oA_rad / 2 + oB_rad / 2;
	float distBtwPoints = sqrt((oA_x - oB_x)*(oA_x - oB_x) + (oA_y - oB_y)*(oA_y - oB_y));


	if (colLenLimit - distBtwPoints < FLT_EPSILON)
		return false;

	return true;
}

//이 부분은 ScnMgr로 넘어감
/*
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
*/

/*
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
*/


char WallCollision(const float& posX, const float& posY, const float& rad, const float& height)
{
	if (posX - rad / 2 <= -WINDOW_SIZEX / 2 || posX + rad / 2 >= WINDOW_SIZEX / 2) {
		//Right or Left
		return 'r';
	}
	if (posY - rad / 2 <= -WINDOW_SIZEY / 2 || posY + rad / 2 >= WINDOW_SIZEY / 2) {
		//Up or Down
		return 'u';
	}

	//return none
	return 'n';
}

bool JoinCollision(const float& obj_Rad ,const float& obj_PosX, const float& obj_PosY, const float& posx, const float& posy) {
	
	return CollisionCheck(obj_Rad, obj_PosX, obj_PosY, posx, posy);
}