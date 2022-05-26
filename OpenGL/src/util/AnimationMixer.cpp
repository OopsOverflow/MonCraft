#include "AnimationMixer.hpp"
#include <algorithm>

AnimationMixer::AnimationMixer(Spline idleAnim) : currentAnimation(Animation::Idle), nextAnimation(Animation::Idle) {
	addAnim(Animation::Idle, idleAnim);
}

void AnimationMixer::addAnim(Animation animType, Spline relatedAnimation) {
	animations.emplace(animType,  relatedAnimation);
}

glm::vec3 AnimationMixer::computeAnim(float dt) {
	glm::vec3 target;
	while (dt > 0)
	{
        if(transition != nullptr) {
			target = transition->compute(dt);
			if(dt > 0) transition = nullptr;
		}
        else {
            target = animations.at(currentAnimation).compute(dt);
            if(dt > 0) {
                std::vector<std::pair<float, glm::vec3> > keyframes;
				Spline* current = &animations.at(currentAnimation);
				Spline* next = &animations.at(nextAnimation);

				float transitionTime = distance(current->getCurrentPoint(), next->getCurrentPoint()) / std::max(current->getCurrentSpeed(), next->getCurrentSpeed());

				keyframes.emplace_back(0.f, current->getCurrentPoint());
				keyframes.emplace_back(transitionTime, next->getCurrentPoint());
				transition = std::make_unique<Spline>(keyframes, smoothing * current->getCurrentDerivative(), smoothing * -next->getCurrentDerivative());
				
				current->reset();
				currentAnimation = nextAnimation;
				nextAnimation = Animation::Idle;
            } 
		}
	}
	return target;

}

void AnimationMixer::setAnimation(Animation anim) {
	if(currentAnimation != anim && (currentAnimation != Animation::Break || anim == Animation::Break)) {
		std::vector<std::pair<float, glm::vec3> > keyframes;
		Spline* current;
		if(transition != nullptr)
			current = transition.get();
		else
			current = &animations.at(currentAnimation);
			
        Spline* next = &animations.at(anim);
		
		float transitionTime = distance(current->getCurrentPoint(), next->getCurrentPoint()) * 2.f / ( current->getCurrentSpeed() + next->getCurrentSpeed() );

		keyframes.emplace_back(0.f, current->getCurrentPoint());
		keyframes.emplace_back(transitionTime, next->getCurrentPoint());
		transition = std::make_unique<Spline>(keyframes, smoothing * current->getCurrentDerivative(), smoothing * -next->getCurrentDerivative());
		
		current->reset();
		currentAnimation = anim;
	}else 
	{
		nextAnimation = anim;
	}
	
}
