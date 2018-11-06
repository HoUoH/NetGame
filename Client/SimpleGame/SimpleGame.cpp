/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/


#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "ScnMgr.h"
#include "join.h"

ScnMgr *g_ScnMgr = NULL;
DWORD prev_render_time = 0;
BOOL W_KeyIsDown = false;
BOOL A_KeyIsDown = false;
BOOL S_KeyIsDown = false;
BOOL D_KeyIsDown = false;

int GitTest;

void RenderScene(void)	//1초에 최소 60번 이상 출력되어야 하는 함수
{
	if (prev_render_time == 0)
		prev_render_time = timeGetTime();
	//Elapsed Time
	DWORD current_time = timeGetTime();
	DWORD elapsed_time = current_time - prev_render_time;
	prev_render_time = current_time;
	float eTime = elapsed_time / 1000.f;//convert to second
//	std::cout << "elapsedTime:" << eTime << std::endl;
	//std::cout << "w:" << W_KeyIsDown << " a:" << A_KeyIsDown << " s:" << S_KeyIsDown << " d:" << D_KeyIsDown << std::endl;

	float forceX = 0.f, forceY = 0.f;
	float amount = 3000.0f;		// 1N = 1000 기준

	if (W_KeyIsDown)
		forceY += amount;
	if (A_KeyIsDown)
		forceX -= amount;
	if (S_KeyIsDown)
		forceY -= amount;
	if (D_KeyIsDown)
		forceX += amount;

	g_ScnMgr->ApplyForce(forceX, forceY, eTime);
	g_ScnMgr->BreakMovement(W_KeyIsDown, S_KeyIsDown, D_KeyIsDown, A_KeyIsDown, eTime);
	g_ScnMgr->Update(eTime);

	// 클라입장
	// send() 하고 / 서버가 계산한 최종 위치 recv()

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
	//시작 키를 눌렀을 경우 (F1키)
	g_ScnMgr->joinClick(key);
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_SIZEX, WINDOW_SIZEY);
	glutCreateWindow("2018 Network Game Programming KPU");

	glewInit();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutSpecialFunc(SpecialKeyInput);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutMouseFunc(MouseInput);

	g_ScnMgr = new ScnMgr();
	glutMainLoop();
	delete g_ScnMgr;

	return 0;
}

