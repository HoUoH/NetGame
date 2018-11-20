#pragma once

#define SERVERPORT 9000
#define BUFSIZE    512

#define PlAYER_NUM 4
#define BALL_NUM 10
#define MAX_OBJECTS PlAYER_NUM + BALL_NUM
#define HERO_ID 0

#define KIND_HERO 0
#define KIND_BALL 1

#define WINDOW_SIZEX 600
#define WINDOW_SIZEY 600

#define PLAYER_SIZE 25
#define BALL_SIZE 15

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
