#ifndef MATH_EXTRATOOLS
#define MATH_EXTRATOOLS

int FpsToMs(int IntendedRate);

double WrapMax(double Val, double Max);//Val -> [0,Max)
double WrapMinMax(double Val, double Min, double Max);//Val -> [Min,Max)

double Clamp(double x, double Lower=0, double Higher=1); 
double SmoothStep (double LowerBound, double HigherBound, double x);
double SmootherStep(float LowerBound, double HigherBound, double x);

#endif// MATH_EXTRATOOLS