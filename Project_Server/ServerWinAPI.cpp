#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include <stdlib.h>

#include "ServerWinAPI.h"
#include "ScnMgrThread.h"


sendData toSendData[MAX_OBJECTS];
recvData recvedData[PlAYER_NUM];

//메인문
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	InitializeCriticalSection(&cs);

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = (LPCWSTR)"MyWndClass";
	if (!RegisterClass(&wndclass)) return 1;

	//초기화
	for (int i = 0; i < PlAYER_NUM; ++i) 
	{
		toSendData[i].isVisible = false;
		toSendData[i].posX = 0;
		toSendData[i].posY = 0;
		//toSendData[i].velX = 0;
		//toSendData[i].velY = 0;

		recvedData[i].isVisible = false;
		recvedData[i].posX = 0;
		recvedData[i].posY = 0;
		//recvedData[i].velX = 0;
		//recvedData[i].velY = 0;
	}

	// 윈도우 생성
	HWND hWnd = CreateWindow(wndclass.lpszClassName, TEXT("TCP 서버"), WS_OVERLAPPEDWINDOW,
		0, 0, 600, 200, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 소켓 통신 스레드 생성
	hServerMainThread = CreateThread(NULL, 0, ServerMainThread, NULL, 0, NULL);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DeleteCriticalSection(&cs);
	return (int)msg.wParam;	// (메시지 떠서 (int)로 강제 형변환 해줌)
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		hEdit = CreateWindow((LPCWSTR)"edit", NULL,
			WS_CHILD | WS_VISIBLE | WS_HSCROLL |
			WS_VSCROLL | ES_AUTOHSCROLL |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 편집 컨트롤 출력 함수
void DisplayText(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char cbuf[BUFSIZE + 256];
	vsprintf(cbuf, fmt, arg);

	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);

	va_end(arg);
}

// TCP 서버 시작 부분
DWORD WINAPI ServerMainThread(LPVOID arg)
{
	hScnMgrThread = CreateThread(NULL, 0, ScnMgrThread, NULL, 0, NULL);

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	sockAttr sockInfo;
	SOCKADDR_IN clientaddr;
	int addrlen;
	while (1) {
	

			// accept()
			addrlen = sizeof(clientaddr);
			sockInfo.client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
			if (sockInfo.client_sock == INVALID_SOCKET) {
				err_display("accept()");
				break;
			}
			sockInfo.threadNum = FindEmptyThreadSlot();

			if (sockInfo.threadNum == -1) {
				closesocket(sockInfo.client_sock);
				continue;
			}
			// 접속한 클라이언트 정보 출력
			DisplayText("\r\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			// 스레드 생성
			hSRThread[sockInfo.threadNum] = CreateThread(NULL, 0, SendRecvThread, (LPVOID)&sockInfo, 0, NULL);
		
	}
	/*
	for(int i=0;i<PlAYER_NUM;i++)
		CloseHandle(hSRThread[i]);*/
		// closesocket()
	closesocket(listen_sock);
	CloseHandle(hScnMgrThread);
	// 윈속 종료
	WSACleanup();
	return 0;
}

//남은 슬롯을 찾는 함수
int FindEmptyThreadSlot()
{
	for (int i = 0; i < PlAYER_NUM; i++)
	{
		if (isExistPlayer[i] == false)
			return i;
	}
	std::cout << "Object list is full. \n";
	return -1;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 클라이언트와 데이터 통신
DWORD WINAPI SendRecvThread(LPVOID arg)
{
	sockAttr* sockInfoPtr = (sockAttr*)arg;
	sockAttr sockInfo = *sockInfoPtr;

	int threadNum = sockInfo.threadNum;

	////////////////////////////////////
	// ServerWinAPI.h에 있는 전역변수임
	isExistPlayer[threadNum] = true;
	////////////////////////////////////

	//SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(sockInfo.client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	//클라에게 자기 번호 보냄
	retval = send(sockInfo.client_sock, (char*)&threadNum, sizeof(int), 0);

	// 데이터 send(), recv()
	while (1) {
		WaitForSingleObject(hScnMgrEvent, INFINITE);
		// (고정)

		// toSendData "데이터" 보내기
		for (int i = 0; i < MAX_OBJECTS; i++) {
			retval = send(sockInfo.client_sock, (char*)&toSendData[i], sizeof(sendData), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		// 데이터 받기
		retval = recvn(sockInfo.client_sock, (char*)&recvedData[threadNum], sizeof(recvData), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// INFINITE로 무한정 기다릴 수는 없으니
		// 0.01초 안에 ScnMgrEvent가 신호상태로 바뀌지 않으면
		// 무시하고 다시 send() recv()함수 호출한다.
		SetEvent(hSREvent[threadNum]);
//		WaitForSingleObject(hScnMgrEvent, INFINITE);
	}

	// closesocket()
	closesocket(sockInfo.client_sock);
	isExistPlayer[threadNum] = false;

	return 0;
}

//그리는 함수
DWORD prev_render_time = 0;

DWORD WINAPI ScnMgrThread(LPVOID arg)
{
	hScnMgrEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	for (int i = 0; i < PlAYER_NUM; i++)
		hSREvent[i] = CreateEvent(NULL, TRUE, TRUE, NULL);

	ScnMgr *Scn = new ScnMgr();
	Scn->InitObject();

	while (1) {

		WaitForMultipleObjects(4, hSREvent, TRUE, 10);
		if (prev_render_time == 0)
			prev_render_time = timeGetTime();
		//Elapsed Time
		DWORD current_time = timeGetTime();
		DWORD elapsed_time = current_time - prev_render_time;
		prev_render_time = current_time;
		float eTime = elapsed_time / 1000.f;//convert to second

		// Update() 하기전에 받은 RecvedData로 objs[]들을 초기화 시켜주는 함수
		for (int i = 0; i < PlAYER_NUM; ++i) {
			Scn->SetRecvedData(recvedData, i);
		}
		
		Scn->Update(eTime);
		Scn->ObjectCollision();
		Scn->FinalSendDataUpdate();
		// 11.20 익진 추가
		for (int i = 0; i < MAX_OBJECTS; i++) {
			// 현재 cpp파일의 전역변수임
			toSendData[i] = Scn->ReturnSendData(i);
		}

		for (int i = 0; i < PlAYER_NUM; i++) {
			if (!isExistPlayer[i]) {
				toSendData[i].isVisible = false;
				SetEvent(hSREvent[i]);		//isExistPlayer가 false인거랑 매치되는 이벤트는 무조건 셋 해줌 
			}
		}

		SetEvent(hScnMgrEvent);
	}
	CloseHandle(hScnMgrEvent);
	for (int i = 0; i < PlAYER_NUM; i++)
		CloseHandle(hSREvent[i]);
	delete Scn;

	return 0;
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int recieved;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		recieved = recv(s, ptr, left, flags);
		if (recieved == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (recieved == 0)
			break;
		ptr += recieved;
		left -= recieved;
	}

	return (len - left);
}