#pragma once

#include <vector>
#include <memory>
#include "Spline.hpp"

class Animation {
public:

    Animation(Spline idleLoop);


    glm::vec3 computeAnim(float dt);

    void eventAnim(Spline* eventAnim, float transitionTime);

private:
    std::unique_ptr<Spline> startTransition;
    Spline* eventAnimation;
    std::unique_ptr<Spline> endTransition;

    Spline idleLoop;

};