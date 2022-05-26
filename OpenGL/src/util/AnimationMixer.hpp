#pragma once

#include <unordered_map>
#include <memory>
#include "Spline.hpp"


enum class Animation {
    Idle,
    Walk,
    Break,
    Dab
};

class AnimationMixer {
public:

    AnimationMixer(Spline idleAnim);

    void addAnim(Animation animType, Spline relatedAnimation);

    glm::vec3 computeAnim(float dt);

    void setAnimation(Animation anim);

private:
    //transition + animation
    std::unordered_map<Animation, Spline> animations;
    Animation currentAnimation;
    Animation nextAnimation;
    
    std::unique_ptr<Spline> transition;
    
};