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

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ���� ��Ʈ�� ��� �Լ�
void DisplayText(const char *fmt, ...);

DWORD WINAPI SendRecvThread(LPVOID arg);
DWORD WINAPI ScnMgrThread(LPVOID arg);


HINSTANCE hInst; // �ν��Ͻ� �ڵ�
HWND hEdit; // ���� ��Ʈ��
CRITICAL_SECTION cs; // �Ӱ� ����

DWORD WINAPI ServerMainThread(LPVOID arg);
int FindEmptyThreadSlot();
// ���� ��� �Լ�
void err_quit(const char *msg);
void err_display(const char *msg);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);

DWORD WINAPI SendRecvThread(LPVOID arg);

DWORD WINAPI ScnMgrThread(LPVOID arg);

#endif
