#include "Animation.hpp"
#include <iostream>

Animation::Animation(Spline idleLoop) : idleLoop(idleLoop), eventAnimation(nullptr) {
}

glm::vec3 Animation::computeAnim(float dt) {
	glm::vec3 target;
	while (dt > 0)
	{
		if(startTransition != nullptr) {
			target = startTransition->compute(dt);
			if(dt > 0) startTransition = nullptr;
		}
		else if(eventAnimation != nullptr) {
			target = eventAnimation->compute(dt);
			if(dt > 0) {
				eventAnimation->reset();
				eventAnimation = nullptr;	
			} 
		}
		else if(endTransition != nullptr) {
			target = endTransition->compute(dt);
			if(dt > 0) endTransition = nullptr;
		}
		else {
			target = idleLoop.compute(dt);
			if(dt > 0) idleLoop.reset();
		}
	}
	return target;
}

void Animation::eventAnim(Spline* eventAnim, float transitionTime) {
	glm::vec3 currentPos;
	glm::vec3 currentVec;
	
	if(startTransition != nullptr) {
		currentPos = startTransition->getPoint(startTransition->getSplineTime());
		currentVec = startTransition->getDerivative(startTransition->getSplineTime());
	}
	else if(eventAnimation != nullptr) {
		currentPos = eventAnimation->getPoint(eventAnimation->getSplineTime());
		currentVec = eventAnimation->getDerivative(eventAnimation->getSplineTime());
	}
	else if(endTransition != nullptr) {
		currentPos = endTransition->getPoint(endTransition->getSplineTime());
		currentVec = endTransition->getDerivative(endTransition->getSplineTime());
	}
	else {
		currentPos = idleLoop.getPoint(idleLoop.getSplineTime());
		currentVec = idleLoop.getDerivative(idleLoop.getSplineTime());
	}

	std::vector<std::pair<float, glm::vec3> > keyframes;

	keyframes.emplace_back(0.f, currentPos);
	keyframes.emplace_back(transitionTime, eventAnim->getPoint(0.f));

	startTransition = std::make_unique<Spline>(keyframes, smoothing * currentVec, smoothing * -eventAnim->getDerivative(0.f));

	keyframes = {};
	keyframes.emplace_back(0.f, eventAnim->getPoint(eventAnim->getTotalTime()));
	keyframes.emplace_back(transitionTime, idleLoop.getPoint(0.f));

	endTransition = std::make_unique<Spline>(keyframes, smoothing * eventAnim->getDerivative(eventAnim->getTotalTime()), smoothing * -idleLoop.getDerivative(0.f));

	eventAnimation = eventAnim;

}
