#include "Server_Global.h"

#ifndef WP
#define WP

//����� ���� ����
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
	int kind;					//����
	float posX, posY;			//��ġ
	float sizeX, sizeY;			//ũ��
	float pre_posX, pre_posY;	//���� ��ġ
	float velX, velY;			//�ӵ�
	float VelMag;				//�ӵ�ũ��
	float accX, accY;			//���ӵ�
	float ForceX, ForceY;		//��
	float frictionX, frictionY;	//������
	float mass;					//����
	float coefFriction;			//�������
	float elapsed_time_in_sec;	//����ð�
	bool isVisible;				//ǥ�ÿ���
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
