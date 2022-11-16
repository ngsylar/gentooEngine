#include "Timer.h"

Timer::Timer (float resetTime, float startTime) {
    this->resetTime = resetTime;
    currentTime = startTime;
}

void Timer::SetResetTime (float time) {
    resetTime = time;
}

void Timer::Update (float dt) {
    currentTime += dt;
}

void Timer::Reset () {
    currentTime = 0.0f;
}

float Timer::Get () {
    return currentTime;
}

bool Timer::HasResetTime () {
    return (resetTime > 0.0f);
}

bool Timer::IsOver () {
    return (currentTime >= resetTime);
}