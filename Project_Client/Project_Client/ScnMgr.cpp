#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ScnMgr.h"
#include "Collision.h"
#include "Join.h"
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"



ScnMgr::ScnMgr()
{

	for (int i = 0; i < MAX_OBJECTS; ++i) {
		objs[i] = NULL;
	}
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	m_Renderer = new Renderer(WINDOW_SIZEX, WINDOW_SIZEY);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	
	//이미지 붙여넣기
	Character_Texture[0] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_1.png");
	Character_Texture[1] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_2.png");
	Character_Texture[2] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_3.png");
	Character_Texture[3] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_4.png");
	//플레이어 4명만 함 나머진 보류
	Character_Texture[4] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_5.png");
	Character_Texture[5] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_6.png");
	Character_Texture[6] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_7.png");
	Character_Texture[7] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_8.png");
	Ball_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/PocketBall.png");
	Join_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/JOIN.png");
	Background_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/Background2.png");
	Txt_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/TextTexture.png");

	GameTime = 0.f;
	Invincible_time = 0.f;
	temp = 10.f;
	Invincible_limit = 1;

	for (int i = 0; i < PlAYER_NUM; ++i) {
		objs[i] = new object();
		objs[i]->SetAcc(0, 0);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(1.f);
		objs[i]->SetMass(1.f);
		objs[i]->SetVelocity(0, 0);
		objs[i]->SetSize(PLAYER_SIZE, PLAYER_SIZE);
		objs[i]->SetKind(KIND_HERO);
		objs[i]->SetIsVisible(false);
	}


	for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
		objs[i] = new object();
		objs[i]->SetAcc(0, 0);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(0.5f);
		objs[i]->SetMass(1.f);
		objs[i]->SetVelocity(0, 0);
		objs[i]->SetSize(BALL_SIZE, BALL_SIZE);
		objs[i]->SetKind(KIND_BALL);
		objs[i]->SetIsVisible(false);
	}

}

void ScnMgr::RenderScene()	//1초에 최소 60번 이상 출력되어야 하는 함수
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Renderer->DrawTextureRect(0, 0, 0, 700, 700, 1, 1, 1, 1, Background_Texture);

	float x, y;
	float width, height;

	//캐릭터 그리기
	for (int i = 0; i < PlAYER_NUM; ++i) {
		if (objs[i]->GetIsVisible()) {

			objs[i]->GetLocation(&x, &y);
			objs[i]->GetSize(&width, &height);

			if (i == MyID && Invincible_time < Invincible_limit)
			{
				m_Renderer->DrawTextureRect(x, y, 0, width, height, 0.2, 0, 0, 1, Character_Texture[i]);
			}			
			else
			{
				m_Renderer->DrawTextureRect(x, y, 0, width, height, 1, 1, 1, 1, Character_Texture[i]);
			}
		}
	}

	//볼 그리기
	for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
		if (objs[i]->GetIsVisible()) {
			objs[i]->GetLocation(&x, &y);
			objs[i]->GetSize(&width, &height);
			m_Renderer->DrawTextureRect(x, y, 0, width, height, 1, 1, 1, 1, Ball_Texture);
		}
	}

	m_Renderer->DrawTextureRect(100, 250, 0, 100, 100, 0, 0, 0, 1, Txt_Texture);
	
	char s1[20];
	sprintf(s1, "Survival Time : %d", (int)GameTime);
	glRasterPos2f(0, 0);
	

	for (int i = 0; i < strlen(s1); ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s1[i]);
	}

	RenderJoin();

}

ScnMgr::~ScnMgr()
{

}

void ScnMgr::Update(float elapsed_time_in_sec)
{
	objs[MyID]->Update(elapsed_time_in_sec);
	if (objs[MyID]->GetIsVisible())
	{
		//Invincible_time += elapsed_time_in_sec;

		//디버깅용 무적시간
		Invincible_time += 0;
		GameTime += elapsed_time_in_sec;
	}
}


void ScnMgr::ApplyForce(float ForceX, float ForceY, float elapsed_time_in_sec)
{
	objs[MyID]->ApplyForce(ForceX, ForceY, elapsed_time_in_sec);

}



void ScnMgr::BreakMovement(bool W_KeyIsDown, bool S_KeyIsDown, bool D_KeyIsDown, bool A_KeyIsDown, float elapsed_time_in_sec)
{
	objs[MyID]->BreakMovement(W_KeyIsDown, S_KeyIsDown, D_KeyIsDown, A_KeyIsDown, elapsed_time_in_sec);
}

void ScnMgr::AddObject(float px, float py, float pz, float sx, float sy, float vx, float vy, int Kind)
{

}


void ScnMgr::DeleteObject(unsigned int id)
{

}

void ScnMgr::ObjectCollision()
{
	
	if (objs[MyID]->GetIsVisible()) 
	{
		WallCollision(objs[MyID]);

		if (Invincible_time > Invincible_limit)
		{
			for (int j = PlAYER_NUM; j < MAX_OBJECTS; ++j) 
			{
				if (objs[j]->GetIsVisible()) 
				{
					if (CollisionCheck(objs[MyID], objs[j])) 
					{
						// 충돌에 의한 반응
						CollisionReaction(objs[MyID], objs[j]);
					}
				}
			}
		}

	}
	
}

void ScnMgr::joinClick(char key) {
	
	bool alive = objs[MyID]->GetIsVisible();

	if (alive == true)
		return;

	float posX = 0, posY = 0;

	srand(MyID*100);

	posX = float(rand() % (WINDOW_SIZEX - 100) - 250);
	posY = float(rand() % (WINDOW_SIZEX - 100) - 250);
	objs[MyID]->SetAcc(0.f, 0.f);
	objs[MyID]->SetForce(0.f, 0.f);
	objs[MyID]->SetVelocity(0.f, 0.f);
	objs[MyID]->SetLocation(posX, posY);
	Invincible_time = 0;
	objs[MyID]->SetIsVisible(true);
	GameTime = 0.f;
}

void ScnMgr::RenderJoin() {

	//이거 플레이어 넘버로 바꿔야 함
	BOOL Alive = objs[MyID]->GetIsVisible();
	
	if (death_check(Alive)) {
		m_Renderer->DrawTextureRect(0, 0, 0, 500, 200, 1, 1, 1, 1, Join_Texture);
	}
}

void ScnMgr::SetMyID(int i)
{
	MyID = i;
}

void ScnMgr::UpdateRecvData(float posx, float posy, bool isvisible, int i)
{
	objs[i]->SetLocation(posx, posy);
	objs[i]->SetIsVisible(isvisible);
}

void ScnMgr::getSendData(float * posX, float * posY, bool * isVisible)
{
	objs[MyID]->GetLocation(posX, posY);
	*isVisible = objs[MyID]->GetIsVisible();
}
