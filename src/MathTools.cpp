#include "GentooEngine.h"
#include "MathTools.h"

//Takes intended frames per second and returns approximated time of each frame.
int FpsToMs(int IntendedRate) {
    return 1000/IntendedRate;
}

//Val -> [0,Max).
double WrapMax(double Val, double Max) {
    return fmod(Max + fmod(Val, Max), Max);
}

//Val -> [Min,Max).
double WrapMinMax(double Val, double Min, double Max) {
    return Min + WrapMax(Val - Min, Max - Min);
}

//Limits a value between the [Lower, Higher] range.
double Clamp(double x, double Lower, double Higher) {
    return (x<Lower?Lower:(x>Higher?Higher:x));
}

// Returns a Smoothstep multiplier based on the intended range and current value.
double SmoothStep (double LowerBound, double HigherBound, double x) {
    x = Clamp((x - LowerBound) / (HigherBound - LowerBound));
    return x * x * (3 - 2 * x);
}

// Returns a Smootherstep floating-point multiplier based on intended range and current value. Heavier than Smoothstep.
double SmootherStep(double LowerBound, double HigherBound, double x) {
    x = Clamp((x - LowerBound) / (HigherBound - LowerBound));
    return x * x * x * (x * (x * 6 - 15) + 10);
}