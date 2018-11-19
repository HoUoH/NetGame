#ifndef COLLISION
#define COLLISION

bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y, float posx, float posy);

bool CollisionCheck(const float& oA_rad, const float& oA_x, const float& oA_y,
	const float& oB_rad, const float& oB_x, const float& oB_y);

char WallCollision(const float& posX, const float& posY, const float& rad, const float& height);

bool JoinCollision(const float& obj_Rad, const float& obj_PosX, const float& obj_PosY, const float& posx, const float& posy);

#endif // !COLLISION
