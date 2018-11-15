//#include "ScnMgrThread.h"
//#include "Server_Global.h"
//#include "Server_Handle.h"
//#include "ServerMainThread.h"
#include "Server_Global.h"

#ifndef WP
#define WP

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
