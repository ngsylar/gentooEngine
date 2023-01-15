#include "GentooEngine.h"

ScreenFade::ScreenFade(GameObject& associated, Color filterColor, float start, float finish, float time)
: Component(associated), _FadeFilter(associated, filterColor) {
    type = GameObjID::_ScreenFade;

    _StartPercent = start;
    _CurrPercent = start;
    _FinishPercent = finish;
    _Finished = false;
    _Time = time;

    float Diff = _FinishPercent - _StartPercent;
    (Diff > 0 ? _FadeIn = true : _FadeIn = false);
    _Step = Diff/_Time;
    _FadeFilter.filterColor.A = 255 - 255*_StartPercent;
}

ScreenFade::~ScreenFade() {}

void ScreenFade::RedirectFade(float finish) {
    _StartPercent = _CurrPercent;
    _FinishPercent = finish;
    _Finished = false;
    float Diff = _FinishPercent - _StartPercent;
    (Diff > 0 ? _FadeIn = true : _FadeIn = false);
    _Step = Diff/_Time;
}

void ScreenFade::SetTime(float time) {
    _Time = time;
}

bool ScreenFade::Is(std::string type) {
    return ("ScreenFade" == type);
}

bool ScreenFade::Is(GameObjID type) {
    return (this->type & type);
}

void ScreenFade::Render(){
    _FadeFilter.Render();
}

void ScreenFade::Update(float Dt) {
    if(!_Finished)
    {
        _CurrPercent += _Step*Dt;
        if((_FadeIn && (_CurrPercent > _FinishPercent)) 
        || (!_FadeIn && (_CurrPercent < _FinishPercent)))
        {
            _CurrPercent = _FinishPercent;
            _Finished = true;
        }
        _FadeFilter.filterColor.A = 255 - 255*_CurrPercent;
        _FadeFilter.Update(Dt);
    }
}
