#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

#define PlAYER_NUM 8
#define BALL_NUM 8
#define MAX_OBJECTS PlAYER_NUM + BALL_NUM
#define HERO_ID 0

#define KIND_HERO 0
#define KIND_BALL 1

#define WINDOW_SIZEX 600
#define WINDOW_SIZEY 600

#define PLAYER_SIZE 25
#define BALL_SIZE 15

#define DEATH_START_TIME 0.5

#pragma pack(1)
struct recvData {
	float posX, posY;
	bool isVisible;
}typedef recvData;
#pragma pack()

#pragma pack(1)
struct sendData_Object {
	float posX, posY;
	bool isVisible;
}typedef sendData_Object;
#pragma pack()

#pragma pack(1)
struct sendData {
	sendData_Object senObj[MAX_OBJECTS];
}typedef sendData;
#pragma pack()

