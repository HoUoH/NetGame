#pragma once
#include <chrono>
#include <iostream>

class TimeCheck {
private:
	std::chrono::steady_clock::time_point beginTime;
public:
	TimeCheck();
	~TimeCheck();
	double ScoreTime();
};