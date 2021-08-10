#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

class Timer {
public:
	Timer();

	void Stop();
	void Start();
	double TimeSinceLast();
	double TimeAfterStop();
	double GetMicro();
	double GetMilis();
	double GetSec();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
	std::chrono::microseconds m_s, m_e;
	std::chrono::microseconds m_duration;
};