#pragma once
#include "ScnMgrThread.h"
#include "Server_Global.h"
#include "Server_Handle.h"
#include "ServerMainThread.h"

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ���� ��Ʈ�� ��� �Լ�
void DisplayText(const char *fmt, ...);

DWORD WINAPI SendRecvThread(LPVOID arg);
DWORD WINAPI ScnMgrThread(LPVOID arg);


HINSTANCE hInst; // �ν��Ͻ� �ڵ�
HWND hEdit; // ���� ��Ʈ��
CRITICAL_SECTION cs; // �Ӱ� ����



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);