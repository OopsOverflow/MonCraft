#pragma once

#include <vector>
#include <memory>
#include "Spline.hpp"

class Animation {
public:

    Animation(Spline idleLoop);

    glm::vec3 computeAnim(float dt);

    void addEventAnim(Spline eventAnim, float transitionTime);
    void overrideEventAnims(Spline eventAnim, float transitionTime);

    size_t eventAnimsSize() {return eventAnimations.size();}
    float eventAnimDuration();

    void stopAnim();

    void setEndTransitionTime(float t) {endTransitionTime = t;}
private:
    //transition + animation
    std::vector<std::pair<Spline, Spline> > eventAnimations;
    std::unique_ptr<Spline> endTransition;
    Spline idleLoop;

    float endTransitionTime;
    
};