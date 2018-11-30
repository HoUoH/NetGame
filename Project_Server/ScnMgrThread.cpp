#include "pch.h"
#include "Server_Global.h"
#include "ScnMgrThread.h"
#include "Collision.h"

//공이 한쪽으로 모이는걸 방지하기 위해 쓴 인자
int FrameCount = 0;
sendData toSendData_InScnMgr[MAX_OBJECTS];
int seq = 0;

ScnMgr::ScnMgr()
{
   InitObject();

}
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
	  toSendData_InScnMgr[i].isVisible = false;
	  toSendData_InScnMgr[i].posX = 0;
	  toSendData_InScnMgr[i].posY = 0;
	  toSendData_InScnMgr[i].velX = 0;
	  toSendData_InScnMgr[i].velY = 0;
   }

   for (int i = 0; i < PlAYER_NUM; ++i) {
      objs[i] = new object();
      objs[i]->SetAcc(0, 0);
      objs[i]->SetForce(0, 0);
      objs[i]->SetCoefFriction(1.f);
      objs[i]->SetMass(1.f);
      objs[i]->SetVelocity(10, 10);
      objs[i]->SetSize(PLAYER_SIZE, PLAYER_SIZE);
      objs[i]->SetKind(KIND_HERO);
      objs[i]->SetIsVisible(FALSE);
   }

   for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
      objs[i] = new object();
      // 랜덤값으로 움직이게 만듦
      objs[i]->SetAcc(rand() % 100 - 50, rand() % 100 - 50);
      objs[i]->SetForce(0, 0);
      objs[i]->SetCoefFriction(0.5f);
      objs[i]->SetMass(1.f);
	  objs[i]->SetVelocity(rand() % 6000 - 3000, rand() % 6000 - 3000);
      //objs[i]->SetVelocity(1, 1);
      objs[i]->SetSize(BALL_SIZE, BALL_SIZE);
      objs[i]->SetKind(KIND_BALL);
      objs[i]->SetIsVisible(true);
   }

   for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
      bool check = true;
      objs[i]->SetLocation(rand() % (WINDOW_SIZEX - 100) - 250, rand() % (WINDOW_SIZEY - 100) - 250);
      for (int j = 0; j < MAX_OBJECTS; ++j) {
         if (i != j) {
			 
			 float obj1_posX, obj1_posY, obj1_rad;
			 float obj2_posX, obj2_posY, obj2_rad;
			 objs[i]->GetLocation(&obj1_posX, &obj1_posY);
			 objs[i]->GetSize(&obj1_rad, &obj1_rad);
			 objs[j]->GetLocation(&obj2_posX, &obj2_posY);
			 objs[j]->GetSize(&obj2_rad, &obj2_rad);

            if (CollisionCheck(obj1_rad, obj1_posX, obj1_posY,
				obj2_rad, obj2_posX, obj2_posY)) {
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
   for (int i = 0; i < MAX_OBJECTS; ++i) {
      if (objs[i]->GetIsVisible()) {
         objs[i]->Update(elapsed_time_in_sec);

      }
   }
      //printf("사이즈 %d\n", sizeof(objs));
}

// ServerWinAPI.cpp의 ScnMgrThread에 업데이트된 sendData를 전달해주기 위한 함수
sendData ScnMgr::ReturnSendData(int playerIndex)
{
	return toSendData_InScnMgr[playerIndex];
}

void ScnMgr::SetRecvedData(recvData* recvedData_InScnMgr, int playerIndex)	//recv 한 데이터를 서버에 덮어씌움
{
	objs[playerIndex]->SetLocation(recvedData_InScnMgr[playerIndex].posX, recvedData_InScnMgr[playerIndex].posY);
	objs[playerIndex]->SetVelocity(recvedData_InScnMgr[playerIndex].velX, recvedData_InScnMgr[playerIndex].velY);
	objs[playerIndex]->SetIsVisible(recvedData_InScnMgr[playerIndex].isVisible);
	// recvData의 specialKey값 읽어서 objs isVisible 초기화 해줘야하는 부분

	//objs[playerIndex]->SetIsVisible()
}

void ScnMgr::ObjectCollision()
{
	float posX, posY;
	float rad, height;
	float vx, vy;
	float obj1_posX, obj1_posY, obj1_rad;
	float obj2_posX, obj2_posY, obj2_rad;

   for (int i = 0; i < MAX_OBJECTS; ++i) {
      if (objs[i]->GetIsVisible()) {
         //WallCollision(objs[i]);
		 objs[i]->GetLocation(&posX, &posY);
		 objs[i]->GetSize(&rad, &height);
		 switch (WallCollision(posX, posY, rad, height))
		 {
		 case 'r':
			 objs[i]->GetPreLocation(&posX, &posY);
			 objs[i]->SetLocation(posX, posY);
			 objs[i]->GetVelocity(&vx, &vy);
			 objs[i]->SetVelocity(-vx * 2.f, vy);
			 break;
		 case 'u':
			 objs[i]->GetPreLocation(&posX, &posY);
			 objs[i]->SetLocation(posX, posY);
			 objs[i]->GetVelocity(&vx, &vy);
			 objs[i]->SetVelocity(vx, -vy * 2.f);
			 break;
		 default:
			 break;
		 }
         for (int j = i+1; j < MAX_OBJECTS; ++j) {
           // if (i != j)
			 // 11.28 익진
			// 이부분 넣으면 캐릭터끼리 끼어서 못움직이는데
			// 캐릭터 끼리는 충돌체크 하지 않는 방법도 생각해보자

			// 위의 내용을 아래에 반영한 것
			 //if (i < PlAYER_NUM) {	
				 if (objs[j]->GetIsVisible()) {
					 objs[i]->GetLocation(&obj1_posX, &obj1_posY);
					 objs[i]->GetSize(&obj1_rad, &obj1_rad);
					 objs[j]->GetLocation(&obj2_posX, &obj2_posY);
					 objs[j]->GetSize(&obj2_rad, &obj2_rad);
					 if (CollisionCheck(obj1_rad, obj1_posX, obj1_posY,
						 obj2_rad, obj2_posX, obj2_posY)) {
						  // 충돌에 의한 반응
						 CollisionReaction(objs[i], objs[j]);
					 }
				 }
			 //}
         }
      }
   }

}

void ScnMgr::FinalSendDataUpdate()
{
	float velX, velY;
	float posX, posY;

	for (int i = 0; i < MAX_OBJECTS; i++) {
		// ServerWinAPI.cpp의 ScnMgrThread에 업데이트된 sendData를 전달해주기 위해서
		if (objs[i]->GetIsVisible())
		{
			toSendData_InScnMgr[i].isVisible = TRUE;
		}
		else
		{
			toSendData_InScnMgr[i].isVisible = FALSE;
		}
		objs[i]->GetVelocity(&velX, &velY);
		objs[i]->GetLocation(&posX, &posY);
		toSendData_InScnMgr[i].posX = posX;
		toSendData_InScnMgr[i].posY = posY;
		toSendData_InScnMgr[i].velX = velX;
		toSendData_InScnMgr[i].velY = velY;
	}
}

//CollisionReaction 컬리전 반응 넣기
void ScnMgr::CollisionReaction(object* oA, object* oB)
{
	int oA_Kind, oB_Kind = 0;
	oA->GetKind(&oA_Kind);
	oB->GetKind(&oB_Kind);

	float oA_X, oA_Y = 0.f;
	float oB_X, oB_Y = 0.f;
	oA->GetPreLocation(&oA_X, &oA_Y);
	oB->GetPreLocation(&oB_X, &oB_Y);

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
		/*
		float dx = oA_X - oB_X;
		float dy = oA_Y - oB_Y;
		float dab = fabsf(sqrt(dx*dx + dy * dy));

		float sinTheta = dy / fabsf(sqrt(dx*dx + dy * dy));
		float cosTheta = dx / fabsf(sqrt(dx*dx + dy * dy));

		float vxAp = (oA_M - 1.f * oB_M) / (oA_M + oB_M)*(oA_VX*cosTheta + oA_VY * sinTheta) +
			(oB_M + 1.f * oB_M) / (oA_M + oB_M)*(oB_VX*cosTheta + oB_VY * sinTheta);
		float vxBp = (oA_M + 1.f * oA_M) / (oA_M + oB_M)*(oA_VX*cosTheta + oA_VY * sinTheta) +
			(oB_M - 1.f * oA_M) / (oA_M + oB_M)*(oB_VX*cosTheta + oB_VY * sinTheta);

		float vyAp = oA_VX * (-sinTheta) + oA_VY * cosTheta;
		float vyBp = oB_VX * (-sinTheta) + oB_VY * cosTheta;

		oA->SetVelocity(vxAp, vyAp);
		oB->SetVelocity(vyAp, vyBp);
		oA->SetLocation(oA_X, oA_Y);
		oB->SetLocation(oB_X, oB_Y);
		*/
	}
	else
	{
	
		if (oA_Kind == KIND_HERO)
		{
			oA->SetIsVisible(false);
		}
		else if (oB_Kind == KIND_HERO) {
			oB->SetIsVisible(false);
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
