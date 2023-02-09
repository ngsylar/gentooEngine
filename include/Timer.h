#ifndef TIMER_H
#define TIMER_H

class Timer {
    private:
        float resetTime, currentTime;
    
    public:
        Timer(float resetTime=0.0f, float startTime=0.0f);
        void SetResetTime(float time);
        void FalseStart();
        void Update(float dt);
        void Reset();
        float GetTime();
        float GetResetTime();
        bool HasResetTime();
        bool IsOver();
        bool IsBetween(float timeStart, float timeEnd);
};

#endif