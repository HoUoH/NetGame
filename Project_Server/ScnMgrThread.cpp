#include "pch.h"
#include "Server_Global.h"
#include "ScnMgrThread.h"
#include "Collision.h"

//���� �������� ���̴°� �����ϱ� ���� �� ����
int FrameCount = 0;
sendData toSendData_InScnMgr[MAX_OBJECTS];
int seq = 0;

ScnMgr::ScnMgr()
{
   InitObject();

}
void ScnMgr::RenderScene()   //1�ʿ� �ּ� 60�� �̻� ��µǾ�� �ϴ� �Լ�
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
      // ���������� �����̰� ����
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
   //�� �κп� �������� ���� ������ objs[]�� �ֽ�ȭ�ؾߵ�(���ڸ��� �ֽ�ȭ�ϴ� �͵� ���)
	
   //PlAYER_NUM+1  : ������Ʈ ������ �ֽ�ȭ
   for (int i = 0; i < MAX_OBJECTS; ++i) {
      if (objs[i]->GetIsVisible()) {
         objs[i]->Update(elapsed_time_in_sec);

      }
   }
      //printf("������ %d\n", sizeof(objs));
}

// ServerWinAPI.cpp�� ScnMgrThread�� ������Ʈ�� sendData�� �������ֱ� ���� �Լ�
sendData ScnMgr::ReturnSendData(int playerIndex)
{
	return toSendData_InScnMgr[playerIndex];
}

void ScnMgr::SetRecvedData(recvData* recvedData_InScnMgr, int playerIndex)	//recv �� �����͸� ������ �����
{
	objs[playerIndex]->SetLocation(recvedData_InScnMgr[playerIndex].posX, recvedData_InScnMgr[playerIndex].posY);
	objs[playerIndex]->SetVelocity(recvedData_InScnMgr[playerIndex].velX, recvedData_InScnMgr[playerIndex].velY);
	objs[playerIndex]->SetIsVisible(recvedData_InScnMgr[playerIndex].isVisible);
	// recvData�� specialKey�� �о objs isVisible �ʱ�ȭ ������ϴ� �κ�

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
			 // 11.28 ����
			// �̺κ� ������ ĳ���ͳ��� ��� �������̴µ�
			// ĳ���� ������ �浹üũ ���� �ʴ� ����� �����غ���

			// ���� ������ �Ʒ��� �ݿ��� ��
			 //if (i < PlAYER_NUM) {	
				 if (objs[j]->GetIsVisible()) {
					 objs[i]->GetLocation(&obj1_posX, &obj1_posY);
					 objs[i]->GetSize(&obj1_rad, &obj1_rad);
					 objs[j]->GetLocation(&obj2_posX, &obj2_posY);
					 objs[j]->GetSize(&obj2_rad, &obj2_rad);
					 if (CollisionCheck(obj1_rad, obj1_posX, obj1_posY,
						 obj2_rad, obj2_posX, obj2_posY)) {
						  // �浹�� ���� ����
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
		// ServerWinAPI.cpp�� ScnMgrThread�� ������Ʈ�� sendData�� �������ֱ� ���ؼ�
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

//CollisionReaction �ø��� ���� �ֱ�
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
