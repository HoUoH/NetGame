#include "stdafx.h"
#include "TimeCheck.h"

TimeCheck::TimeCheck() {
	beginTime = std::chrono::high_resolution_clock::now();
}

TimeCheck::~TimeCheck() {
	delete this;
}

double TimeCheck::ScoreTime() {
	auto elapsedtime = std::chrono::high_resolution_clock::now() - beginTime;
	std::cout << std::chrono::duration<double>(elapsedtime).count() << "ÃÊ °æ°ú" << std::endl;
	return std::chrono::duration<double>(elapsedtime).count();
}