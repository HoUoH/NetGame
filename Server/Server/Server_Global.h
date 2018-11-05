#pragma once
#include "pch.h"

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




struct sendData {
	int kind;
	float posX, posY;
	bool isVisible;
}typedef sendData;

struct recvData {
	float posX, posY;
	float velX, velY;
	u_char specialKey;	
}typedef recvData;

struct objects {
	int kind;					//종류
	float posX, posY;			//위치
	float sizeX, sizeY;			//크기
	float pre_posX, pre_posY;	//이전 위치
	float velX, velY;			//속도
	float VelMag;				//속도크기
	float accX, accY;			//가속도
	float ForceX, ForceY;		//힘
	float frictionX, frictionY;	//마찰력
	float mass;					//질량
	float coefFriction;			//마찰계수
	float elapsed_time_in_sec;	//경과시간
	bool isVisible;				//표시여부
}typedef objects;
