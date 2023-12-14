#include "Timer.h"

Timer& Timer::GetInstance()
{
    static Timer instance;
    return instance;
}

void Timer::SetCurrentTime(float currentTime)
{
    this->currentTime = currentTime;

    deltaTime = currentTime - lastFrameTime;

    lastFrameTime = currentTime;
}
