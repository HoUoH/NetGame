#pragma once

#pragma comment(lib, "ws2_32")

#include "Server_Handle.h"
#include "Server_Global.h"

// 오류 출력 함수
void err_quit(const char *msg);
void err_display(const char *msg);
int FindEmptyThreadSlot();
// 소켓 통신 스레드 함수
DWORD WINAPI ServerMainThread(LPVOID arg);

