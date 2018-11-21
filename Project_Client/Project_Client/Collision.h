#pragma once
bool CollisionCheck(class object* oA, class object* oB);

bool CollisionCheck(class object * oA, float posx, float posy);

void CollisionReaction(class object* objA, class object* objB);

void WallCollision(class object* obj);

bool JoinCollision(class object* obj, float posx, float posy);