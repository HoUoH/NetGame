#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "ScnMgr.h"
#include "resource.h"
#include "stdafx.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

float eTime = 0;


// ��ȭ���� ���ν���
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// ���� ��Ʈ�� ��� �Լ�
void DisplayText(char *fmt, ...);
// ���� ��� �Լ�
void err_quit(char *msg);
void err_display(char *msg);
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);
// ���� ��� ������ �Լ�
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock; // ����
char addrBuf[BUFSIZE + 1]; // ���� IP �ּ�
char fileBuf[BUFSIZE + 1]; // �������� ���� ���� �̸�
HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ
HANDLE drawEvent;
HWND hSendButton; // ������ ��ư
HWND hEdit1, hEdit2; // ���� ��Ʈ��

ScnMgr *g_ScnMgr = NULL;
DWORD prev_render_time = 0;
BOOL W_KeyIsDown = false;
BOOL A_KeyIsDown = false;
BOOL S_KeyIsDown = false;
BOOL D_KeyIsDown = false;

// ��ȭ���� ���ν���
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���� ��Ʈ�� ��� �Լ�
void DisplayText(char *fmt, ...);

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg);

//�� ������ ��� �Լ�
void RenderScene(void);

void Idle(void);

//Ű input ����
void MouseInput(int button, int state, int x, int y);

void KeyDownInput(unsigned char key, int x, int y);

void KeyUpInput(unsigned char key, int x, int y);

void SpecialKeyInput(int key, int x, int y);

DWORD WINAPI DrawMain(LPVOID arg);


#pragma pack(1)
struct recvData {
	int kind;
	float posX, posY;
	bool isVisible;
}typedef recvData;
#pragma pack()

#pragma pack(1)
struct sendData {
	float posX, posY;
	float velX, velY;
	int specialKey;
}typedef sendData;
#pragma pack()

sendData tosendData;

int sendkey;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hReadEvent == NULL) return 1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL) return 1;

	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);

	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	return 0;
}

// ��ȭ���� ���ν���
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
		hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
		hSendButton = GetDlgItem(hDlg, IDOK);
		SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			SetDlgItemText(hDlg, IDC_EDIT2, "");
			EnableWindow(hSendButton, FALSE); // ������ ��ư ��Ȱ��ȭ
			WaitForSingleObject(hReadEvent, INFINITE); // �̺�Ʈ ��ٸ���
			GetDlgItemText(hDlg, IDC_EDIT1, addrBuf, BUFSIZE + 1);
			DisplayText(addrBuf);
			SetEvent(hWriteEvent); // ���� �Ϸ� �˸���
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

// ���� ��Ʈ�� ��� �Լ�
void DisplayText(char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	int nLength = GetWindowTextLength(hEdit2);
	SendMessage(hEdit2, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit2, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

	va_end(arg);
}

// ���� �Լ� ���� ��� �� ����0
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText((char*)"[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{
	//��� �Է��� �޾ƾ� �ϱ� ������ while�ȿ� �ִ´�

		int retval;
		//Ŭ�� ��ٸ���
	while (1) {
		WaitForSingleObject(hWriteEvent, INFINITE);

		int len = 0;
		char buf[BUFSIZE] = { NULL };

		// ���� �ʱ�ȭ
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		// socket()
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit((char*)"socket()");

		// connect()
		SOCKADDR_IN serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(addrBuf);
		serveraddr.sin_port = htons(SERVERPORT);
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR)
		{
			err_display((char*)"connect()");
			EnableWindow(hSendButton, TRUE); // ������ ��ư Ȱ��ȭ
			SetEvent(hReadEvent); // �б� �Ϸ� �˸���
		}
		else {
			CreateThread(NULL, 0, DrawMain, NULL, 0, NULL);
			drawEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	//Scnmgr �ʱ�ȭ ��ٸ�
			if (drawEvent == NULL) return 1;
			break;
		}
	}
		WaitForSingleObject(drawEvent, INFINITE);
		// ������ ������ ���
		//�ڱ� ��ȣ �ޱ�
		int id;
		retval = recvn(sock, (char*)&id, sizeof(int), 0);
		g_ScnMgr->SetMyID(id);
		recvData recvedData[MAX_OBJECTS];
		//sendData tosendData;
		while (1) {
			for (int i = 0; i < MAX_OBJECTS; i++) {
				//��ü ������ �ޱ�
				retval = recvn(sock, (char*)&recvedData[i], sizeof(recvData), 0);
				if (retval == SOCKET_ERROR) {
					err_display((char*)"recv()");
					break;
				}
				//if(i!=id)
				//g_ScnMgr->Update(eTime);
				g_ScnMgr->UpdateRecvData(recvedData[i].isVisible, recvedData[i].posX, recvedData[i].posY,i);
			}

			g_ScnMgr->getSendData(&(tosendData.posX), &(tosendData.posY), &(tosendData.velX), &(tosendData.velY));
			tosendData.specialKey = sendkey;
			sendkey = 0;

				//�ڱ� ��ġ ������
				retval = send(sock, (char*)&tosendData, sizeof(sendData), 0);
				if (retval == SOCKET_ERROR) {
					err_display((char*)"send()");
					break;
				}
			
		}
		//	EnableWindow(hSendButton, TRUE); // ������ ��ư Ȱ��ȭ
			SetEvent(hReadEvent); // �б� �Ϸ� �˸���


			// closesocket()
			closesocket(sock);

			// ���� ����
			WSACleanup();
			DisplayText((char*)"������ �ޱ� ��!\r\n");
	
	return 0;
}

void RenderScene(void)	//1�ʿ� �ּ� 60�� �̻� ��µǾ�� �ϴ� �Լ�
{
	if (prev_render_time == 0)
		prev_render_time = timeGetTime();
	//Elapsed Time
	DWORD current_time = timeGetTime();
	DWORD elapsed_time = current_time - prev_render_time;
	prev_render_time = current_time;
	eTime = elapsed_time / 1000.f;//convert to second


	float forceX = 0.f, forceY = 0.f;
	float amount = 3000.0f;		// 1N = 1000 ����

	if (W_KeyIsDown)
		forceY += amount;
	if (A_KeyIsDown) {
		forceX -= amount;
		tosendData.posX += 10;

	}
	if (S_KeyIsDown)
		forceY -= amount;
	if (D_KeyIsDown)
		forceX += amount;

	g_ScnMgr->ApplyForce(forceX, forceY, eTime);
	g_ScnMgr->BreakMovement(W_KeyIsDown, S_KeyIsDown, D_KeyIsDown, A_KeyIsDown, eTime);
	g_ScnMgr->Update(eTime);

	// Ŭ������
	// send() �ϰ� / ������ ����� ���� ��ġ recv()

	g_ScnMgr->RenderScene();
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyDownInput(unsigned char key, int x, int y)
{
	float amount = 0.1f;
	switch (key) {
	case 'w':
		W_KeyIsDown = TRUE;
		break;
	case 'a':
		A_KeyIsDown = TRUE;
		tosendData.posX += 10;
		break;
	case 's':
		S_KeyIsDown = TRUE;
		break;
	case 'd':
		D_KeyIsDown = TRUE;
		break;
	default:
		break;
	}
}

void KeyUpInput(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		W_KeyIsDown = FALSE;
		break;
	case 'a':
		A_KeyIsDown = FALSE;
		break;
	case 's':
		S_KeyIsDown = FALSE;
		break;
	case 'd':
		D_KeyIsDown = FALSE;
		break;
	default:

		break;
	}
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	//���� Ű�� ������ ��� (F1Ű)
	if (key == 1) {
		sendkey = 1;
	}
	RenderScene();
}

DWORD WINAPI DrawMain(LPVOID arg) {
	
	int argc = 1;
	char* argv[1] = {(char*)"Project_Client.exe"};	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_SIZEX, WINDOW_SIZEY);
	glutCreateWindow("KPU");

	glewInit();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutSpecialFunc(SpecialKeyInput);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutMouseFunc(MouseInput);

	g_ScnMgr = new ScnMgr();
	SetEvent(drawEvent);
	while (1)
	{
		glutMainLoop();
	}
	delete g_ScnMgr;

	return 0;
}