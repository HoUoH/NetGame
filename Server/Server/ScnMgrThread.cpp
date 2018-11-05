#include "pch.h"

#include "ScnMgrThread.h"
//#include "Join.h"

//공이 한쪽으로 모이는걸 방지하기 위해 쓴 인자
int FrameCount = 0;

ScnMgr::ScnMgr()
{

	for (int i = 0; i < MAX_OBJECTS; ++i) {
		objs[i] = NULL;
	}
	
	for (int i = 0; i < PlAYER_NUM; ++i) {
		objs[i] = new object();
		objs[i]->SetAcc(0, 0);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(1.f);
		objs[i]->SetMass(4.f);
		objs[i]->SetVelocity(10, 10);
		objs[i]->SetSize(PLAYER_SIZE, PLAYER_SIZE);
		objs[i]->SetKind(KIND_HERO);
		objs[i]->SetIsVisible(true);
	}

	objs[HERO_ID]->SetIsVisible(false);

	for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
		objs[i] = new object();
		// 랜덤값으로 움직이게 만듦
		objs[i]->SetAcc(rand() % 100 - 50, rand() % 100 - 50);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(0.5f);
		objs[i]->SetMass(1.f);
		objs[i]->SetVelocity(rand() % 6000 - 3000, rand() % 6000 - 3000);
		objs[i]->SetSize(BALL_SIZE, BALL_SIZE);
		objs[i]->SetKind(KIND_BALL);
		objs[i]->SetIsVisible(true);
	}

	// MAX_OBJECTS - 4는 벽을 제외하기 위해서
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		bool check = true;
		objs[i]->SetLocation(rand() % (WINDOW_SIZEX - 100) - 250, rand() % (WINDOW_SIZEY - 100) - 250);
		for (int j = 0; j < MAX_OBJECTS; ++j) {
			if (i != j) {
				if (CollisionCheck(objs[i], objs[j])) {
					check = false;
					break;
				}
			}
		}
		if (check == false) {
			i--;
			continue;
		}
	}


}
int seq = 0;
void ScnMgr::RenderScene()	//1초에 최소 60번 이상 출력되어야 하는 함수
{
	
}

ScnMgr::~ScnMgr()
{

}

float temp = 10.f;

void ScnMgr::Update(float elapsed_time_in_sec)
{
	ObjectCollision();
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		if (objs[i]->GetIsVisible()) {
			objs[i]->Update(elapsed_time_in_sec);
		}
	}
	//printf("사이즈 %d\n", sizeof(objs));
}


void ScnMgr::ApplyForce(float ForceX, float ForceY, float elapsed_time_in_sec)
{
	objs[HERO_ID]->ApplyForce(ForceX, ForceY, elapsed_time_in_sec);

}



void ScnMgr::BreakMovement(bool W_KeyIsDown, bool S_KeyIsDown, bool D_KeyIsDown, bool A_KeyIsDown, float elapsed_time_in_sec)
{
	objs[HERO_ID]->BreakMovement(W_KeyIsDown, S_KeyIsDown, D_KeyIsDown, A_KeyIsDown, elapsed_time_in_sec);
}

void ScnMgr::AddObject(float px, float py, float pz, float sx, float sy, float vx, float vy, int Kind)
{

}


void ScnMgr::DeleteObject(unsigned int id)
{

}

void ScnMgr::ObjectCollision()
{

	for (int i = 0; i < MAX_OBJECTS; ++i) {
		if (objs[i]->GetIsVisible()) {
			WallCollision(objs[i]);
			for (int j = 0; j < MAX_OBJECTS; ++j) {
				if (i != j)
					if (objs[j]->GetIsVisible()) {
						if (CollisionCheck(objs[i], objs[j])) {
							float posX, posY = 0;
							objs[i]->GetPreLocation(&posX, &posY);
							objs[i]->SetLocation(posX, posY);
							objs[j]->GetPreLocation(&posX, &posY);
							objs[j]->SetLocation(posX, posY);
							// 충돌에 의한 반응
							CollisionReaction(objs[i], objs[j]);

							//캐릭터 - 공(떨궈진 공-먹어지기, 쏜 공-, 먹은 공)

							//공 - 공

							//DeleteObject(j);
							//std::cout << "collision" << std::endl;
						}
					}
			}
		}
	}

}


int ScnMgr::FindEmptyObjectSlot()
{
	for (int i = 0; i < MAX_OBJECTS; ++i) {
		if (objs[i] == NULL)
			return i;
	}
	std::cout << "object list is full.\n";
	return -1;
}

void ScnMgr::joinClick(int key) {

	}

void ScnMgr::RenderJoin() {

}

void InitObject()
{
}

void ObjectUpdate()
{
}

void ObjectCollision()
{
}
DWORD WINAPI ScnMgrThread(LPVOID arg)
{
	InitObject();
	while (1) {
		WaitForMultipleObjects(8, &SREvent, true, INFINITE);
		ObjectUpdate();
		ObjectCollision();
		SetEvent(ScnMgrEvent);
	}

	return 0;
}
