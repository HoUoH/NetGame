#include "Server_Global.h"

#ifndef WP
#define WP

//사용할 변수 선언
#pragma pack(1)
struct sendData {
	int kind;
	float posX, posY;
	bool isVisible;
}typedef sendData;
#pragma pack()

#pragma pack(1)
struct recvData {
	float posX, posY;
	float velX, velY;
	u_char specialKey;
}typedef recvData;
#pragma pack()

#pragma pack(1)
struct objects {
	int kind;					//종류
	float posX, posY;			//위치
	float sizeX, sizeY;			//크기
	float pre_posX, pre_posY;	//이전 위치
	float velX, velY;			//속도
	float VelMag;				//속도크기
	float accX, accY;			//가속도
	float ForceX, ForceY;		//힘
	float frictionX, frictionY;	//마찰력
	float mass;					//질량
	float coefFriction;			//마찰계수
	float elapsed_time_in_sec;	//경과시간
	bool isVisible;				//표시여부
}typedef objects;
#pragma pack()

// ServerMainThread
bool isExistPlayer[PlAYER_NUM] = { false };
int playerIndex = 0;

HANDLE hSREvent[PlAYER_NUM];
HANDLE hSRThread[PlAYER_NUM];

HANDLE hScnMgrEvent;
HANDLE hScnMgrThread;

HANDLE hServerMainEvent;
HANDLE hServerMainThread;

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 편집 컨트롤 출력 함수
void DisplayText(const char *fmt, ...);

DWORD WINAPI SendRecvThread(LPVOID arg);
DWORD WINAPI ScnMgrThread(LPVOID arg);


HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 편집 컨트롤
CRITICAL_SECTION cs; // 임계 영역

DWORD WINAPI ServerMainThread(LPVOID arg);
int FindEmptyThreadSlot();
// 오류 출력 함수
void err_quit(const char *msg);
void err_display(const char *msg);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);

DWORD WINAPI SendRecvThread(LPVOID arg);

DWORD WINAPI ScnMgrThread(LPVOID arg);

#endif
