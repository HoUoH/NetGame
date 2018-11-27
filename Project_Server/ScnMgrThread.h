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
	sendData ReturnSendData(int playerIndex);
	void SetRecvedData(recvData* recvedData_InScnMgr, int playerIndex);
	void ObjectCollision();
	void FinalSendDataUpdate();
	int FindEmptyObjectSlot();
	void CollisionReaction(object* oA, object* oB);
private:
	object *objs[MAX_OBJECTS];
};

