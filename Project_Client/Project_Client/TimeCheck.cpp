#include "stdafx.h"
#include "TimeCheck.h"

//디버깅 할때 썻던 시간 체크용 cpp, 현재는 빼져 있음
TimeCheck::TimeCheck() {
	beginTime = std::chrono::high_resolution_clock::now();
}

TimeCheck::~TimeCheck() {
	delete this;
}

double TimeCheck::ScoreTime() {
	auto elapsedtime = std::chrono::high_resolution_clock::now() - beginTime;
	std::cout << std::chrono::duration<double>(elapsedtime).count() << "초 경과" << std::endl;
	return std::chrono::duration<double>(elapsedtime).count();
}