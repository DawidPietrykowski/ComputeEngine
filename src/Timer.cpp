#include "Timer.h"

Timer::Timer() {
	m_start = std::chrono::high_resolution_clock::now();
	m_s = std::chrono::microseconds(0);
	m_e = std::chrono::microseconds(0);
	m_duration = std::chrono::microseconds(0);
}
void Timer::Start() {
	m_start = std::chrono::high_resolution_clock::now();
}
void Timer::Stop() {
	m_end = std::chrono::high_resolution_clock::now();
}
double Timer::TimeSinceLast() {
	m_end = std::chrono::high_resolution_clock::now();
	m_s = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch();
	m_e = std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch();
	m_start = std::chrono::high_resolution_clock::now();
	m_duration = m_e - m_s;
	return (double)m_duration.count();
}
double Timer::GetMicro() {
	return Timer::TimeSinceLast();
}
double Timer::GetMilis() {
	return Timer::TimeSinceLast() * 0.001;
}
double Timer::GetSec() {
	return Timer::TimeSinceLast() * 0.000001;
}
double Timer::TimeAfterStop() {
	m_s = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch();
	m_e = std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch();
	m_duration = m_e - m_s;
	return m_duration.count() * 0.001;
}