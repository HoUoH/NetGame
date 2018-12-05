#include "stdafx.h"
#include "ScnMgr.h"
#include "Collision.h"
#include "Join.h"

//공이 한쪽으로 모이는걸 방지하기 위해 쓴 인자
int FrameCount = 0;
float Invincible_time;

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
	//Character_Texture[4] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_5.png");
	//Character_Texture[5] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_6.png");
	//Character_Texture[6] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_7.png");
	//Character_Texture[7] = m_Renderer->CreatePngTexture((char*)"./Textures/Player_8.png");
	Ball_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/PocketBall.png");
	Join_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/JOIN.png");
	Background_Texture = m_Renderer->CreatePngTexture((char*)"./Textures/Background2.png");


	for (int i = 0; i < PlAYER_NUM; ++i) {
		objs[i] = new object();
		objs[i]->SetAcc(0, 0);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(1.f);
		objs[i]->SetMass(1.f);
		//objs[i]->SetVelocity(10, 10);
		objs[i]->SetVelocity(0, 0);
		objs[i]->SetSize(PLAYER_SIZE, PLAYER_SIZE);
		objs[i]->SetKind(KIND_HERO);
		objs[i]->SetIsVisible(false);
	}


	for (int i = PlAYER_NUM; i < MAX_OBJECTS; ++i) {
		objs[i] = new object();
		// 랜덤값으로 움직이게 만듦
		//objs[i]->SetAcc(rand() % 100 - 50, rand() % 100 - 50);
		objs[i]->SetAcc(0, 0);
		objs[i]->SetForce(0, 0);
		objs[i]->SetCoefFriction(0.5f);
		objs[i]->SetMass(1.f);
		//objs[i]->SetVelocity(rand() % 6000 - 3000, rand() % 6000 - 3000);
		objs[i]->SetVelocity(0, 0);
		objs[i]->SetSize(BALL_SIZE, BALL_SIZE);
		objs[i]->SetKind(KIND_BALL);
		objs[i]->SetIsVisible(false);
	}

	//for (int i = 0; i < MAX_OBJECTS; ++i) {
	//	bool check = true;
	//	objs[i]->SetLocation(rand() % (WINDOW_SIZEX - 100) - 250, rand() % (WINDOW_SIZEY - 100) - 250);
	//	for (int j = i+1; j < MAX_OBJECTS; ++j) {
	//		if (CollisionCheck(objs[i], objs[j])) {
	//			check = false;
	//			break;
	//		}
	//		
	//	}
	//	if (check == false) {
	//		i--;
	//		continue;
	//	}
	//}


}
int seq = 0;
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
			m_Renderer->DrawTextureRect(x, y, 0, width, height, 1, 1, 1, 1, Character_Texture[i]);
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

	RenderJoin();

}

ScnMgr::~ScnMgr()
{

}

float temp = 10.f;

void ScnMgr::Update(float elapsed_time_in_sec)
{
	objs[MyID]->Update(elapsed_time_in_sec);
	if (objs[MyID]->GetIsVisible())
	{
		Invincible_time += elapsed_time_in_sec;
	}
	//printf("사이즈 %d\n", sizeof(objs));
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

		if (Invincible_time > 3) 
		{
			for (int j = PlAYER_NUM; j < MAX_OBJECTS; ++j) 
			{
				// 11.28 익진
				// 이부분 넣으면 캐릭터끼리 끼어서 못움직이는데
				// 캐릭터 끼리는 충돌체크 하지 않는 방법도 생각해보자

				if (objs[j]->GetIsVisible()) 
				{
					if (CollisionCheck(objs[MyID], objs[j])) 
					{
						//float posX, posY = 0;
						//objs[MyID]->GetPreLocation(&posX, &posY);
						//objs[MyID]->SetLocation(posX, posY);
						//objs[j]->GetPreLocation(&posX, &posY);
						//objs[j]->SetLocation(posX, posY);
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

	//if (!alive && key == 'r') {
	posX = float(rand() % (WINDOW_SIZEX - 100) - 250);
	posY = float(rand() % (WINDOW_SIZEX - 100) - 250);
	objs[MyID]->SetAcc(0.f, 0.f);
	objs[MyID]->SetForce(0.f, 0.f);
	objs[MyID]->SetVelocity(0.f, 0.f);
	objs[MyID]->SetLocation(posX, posY);
	Invincible_time = 0;
	objs[MyID]->SetIsVisible(true);
	//}
	//죽었는데 다시시작 키(F1)를 눌렀을 시
	//버그 있는거 같은데 이유를 모르겠네
	/*if (click_join(key, alive)) {
		bool check = true;
		float posX = 0, posY = 0;
		//다시 시작 위치를 잡아줘야 한다.
		//다시 시작 위치를 잡아주는 컬리전 함수
		posX = rand() % (WINDOW_SIZEX - 100) - 250;
		posY = rand() % (WINDOW_SIZEX - 100) - 250;
		objs[MyID]->SetAcc(0, 0);
		objs[MyID]->SetForce(0, 0);
		objs[MyID]->SetVelocity(0, 0);
		objs[MyID]->SetLocation(posX, posY);
		Invincible_time = 0;
		objs[MyID]->SetIsVisible(true);
	}*/
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
	//objs[MyID]->GetVelocity(velX, velY);
	*isVisible = objs[MyID]->GetIsVisible();
}

void ScnMgr::SetPlayerCollision(bool isVisible) 
{
	objs[MyID]->SetIsVisible(isVisible);
}

