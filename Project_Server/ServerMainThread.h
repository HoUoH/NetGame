#pragma once

#pragma comment(lib, "ws2_32")

#include "Server_Handle.h"
#include "Server_Global.h"

// ���� ��� �Լ�
void err_quit(const char *msg);
void err_display(const char *msg);
int FindEmptyThreadSlot();
// ���� ��� ������ �Լ�
DWORD WINAPI ServerMainThread(LPVOID arg);

