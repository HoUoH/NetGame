#include "pch.h"

#include "ScnMgrThread.h"
//#include "Join.h"

//공이 한쪽으로 모이는걸 방지하기 위해 쓴 인자
int FrameCount = 0;

ScnMgr::ScnMgr()
{
   InitObject();

}
int seq = 0;
void ScnMgr::RenderScene()   //1초에 최소 60번 이상 출력되어야 하는 함수
{
   
}

ScnMgr::~ScnMgr()
{

}

void ScnMgr::InitObject()
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
      objs[i]->SetIsVisible(false);
   }

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

float temp = 10.f;

void ScnMgr::Update(float elapsed_time_in_sec)
{
   //이 부분에 서버에서 받은 데이터 objs[]에 최신화해야됨(받자마자 최신화하는 것도 방법)

   //PlAYER_NUM+1  : 오브젝트 움직임 최신화
   for (int i = PlAYER_NUM+1; i < MAX_OBJECTS; ++i) {
      if (objs[i]->GetIsVisible()) {
         objs[i]->Update(elapsed_time_in_sec);
      }
   }
   //printf("사이즈 %d\n", sizeof(objs));
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




DWORD WINAPI ScnMgrThread(LPVOID arg)
{
   ScnMgr *Scn = new ScnMgr();
   Scn->InitObject();
   while (1) {
      WaitForMultipleObjects(8, hSREvent, true, INFINITE);
      Scn->Update(1);
      Scn->ObjectCollision();
      SetEvent(hScnMgrEvent);
   }
   delete Scn;
   return 0;
}