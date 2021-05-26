#include "Timer.h"

float Timer::delta = 0.0;
std::chrono::steady_clock::time_point Timer::start = std::chrono::steady_clock::now();
float& Timed::delta = Timer::delta;
std::deque<Timed*> Timer::timed_objects = {};
