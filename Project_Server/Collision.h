//#include "object.h"
//#include "Server_Global.h"

#ifndef COLLISION
#define COLLISION

//bool CollisionCheck(class object* oA, class object* oB);

//bool CollisionCheck(class object * oA, float posx, float posy);

bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y, float posx, float posy);

bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y,
	const float& oB_rad, const float& oB_x, const float& oB_y);

//void CollisionReaction(class object* objA, class object* objB);

//void WallCollision(class object* obj);

char WallCollision(const float& posX, const float& posY, const float& rad, const float& height);

//char WallCollision(class object* obj);

bool JoinCollision(const float& obj_Rad, const float& obj_PosX, const float& obj_PosY, const float& posx, const float& posy);

//bool JoinCollision(class object* obj, float posx, float posy);

#endif // !COLLISION
