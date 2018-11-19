#include "object.h"
#include "Server_Global.h"

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
	void CollisionReaction(object* oA, object* oB);
private:
	object *objs[MAX_OBJECTS];
};

