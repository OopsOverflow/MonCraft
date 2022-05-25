#include "Animation.hpp"
#include <iostream>

Animation::Animation(Spline idleLoop) : idleLoop(idleLoop), endTransitionTime(0.15f) {
}

glm::vec3 Animation::computeAnim(float dt) {
	glm::vec3 target;
	while (dt > 0)
	{
		if(eventAnimations.size() > 0) {
			if(eventAnimations.at(0).first.getSplineTime() != eventAnimations.at(0).first.getTotalTime()) {
				target = eventAnimations.at(0).first.compute(dt);
			}
			else {
				target = eventAnimations.at(0).second.compute(dt);
				if(dt > 0) {
					eventAnimations.erase(eventAnimations.begin());
				} 
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

void Animation::overrideEventAnims(Spline eventAnim, float transitionTime) {
	glm::vec3 currentPos;
	glm::vec3 currentVec;
	
	if(eventAnimations.size() > 0) {
		if(eventAnimations.at(0).first.getSplineTime() != eventAnimations.at(0).first.getTotalTime()) {
			currentPos = eventAnimations.at(0).first.getPoint(eventAnimations.at(0).first.getSplineTime());
			currentVec = eventAnimations.at(0).first.getDerivative(eventAnimations.at(0).first.getSplineTime());	
		}
		else {
			currentPos = eventAnimations.at(0).second.getPoint(eventAnimations.at(0).second.getSplineTime());
			currentVec = eventAnimations.at(0).second.getDerivative(eventAnimations.at(0).second.getSplineTime());
		}
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
	keyframes.emplace_back(transitionTime, eventAnim.getPoint(eventAnim.getSplineTime()));

	Spline startTransition(keyframes, smoothing * currentVec, smoothing * -eventAnim.getDerivative(eventAnim.getSplineTime()));

	keyframes = {};
	keyframes.emplace_back(0.f, eventAnim.getPoint(eventAnim.getTotalTime()));
	keyframes.emplace_back(endTransitionTime, idleLoop.getPoint(idleLoop.getSplineTime()));

	endTransition = std::make_unique<Spline>(keyframes, smoothing * eventAnim.getDerivative(eventAnim.getTotalTime()), smoothing * -idleLoop.getDerivative(idleLoop.getSplineTime()));

	eventAnimations.clear();
	eventAnimations.push_back({startTransition, eventAnim});

}



void Animation::addEventAnim(Spline eventAnim, float transitionTime) {
	glm::vec3 currentPos;
	glm::vec3 currentVec;
	
	size_t animSize = eventAnimsSize();
	if(animSize > 0) {
		currentPos = eventAnimations.at(animSize - 1).second.getPoint(eventAnimations.at(animSize - 1).second.getTotalTime());
		currentVec = eventAnimations.at(animSize - 1).second.getDerivative(eventAnimations.at(animSize - 1).second.getTotalTime());
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
	keyframes.emplace_back(transitionTime, eventAnim.getPoint(0.f));

	Spline startTransition(keyframes, smoothing * currentVec, smoothing * -eventAnim.getDerivative(eventAnim.getSplineTime()));

	keyframes = {};
	keyframes.emplace_back(0.f, eventAnim.getPoint(eventAnim.getTotalTime()));
	keyframes.emplace_back(endTransitionTime, idleLoop.getPoint(idleLoop.getSplineTime()));

	endTransition = std::make_unique<Spline>(keyframes, smoothing * eventAnim.getDerivative(eventAnim.getTotalTime()), smoothing * -idleLoop.getDerivative(idleLoop.getSplineTime()));

	eventAnimations.push_back({startTransition, eventAnim});

}

float Animation::eventAnimDuration() {
	float res = 0.f;
	for(auto pair : eventAnimations) {
		res += pair.first.getTotalTime() - pair.first.getSplineTime();
		res += pair.second.getTotalTime() - pair.second.getSplineTime();
	}

	return res;
}


void Animation::stopAnim() {
	
	if(eventAnimations.size() > 0) {
		glm::vec3 currentPos;
		glm::vec3 currentVec;
	
		if(eventAnimations.at(0).first.getSplineTime() != eventAnimations.at(0).first.getTotalTime()) {
			currentPos = eventAnimations.at(0).first.getPoint(eventAnimations.at(0).first.getSplineTime());
			currentVec = eventAnimations.at(0).first.getDerivative(eventAnimations.at(0).first.getSplineTime());	
		}
		else {
			currentPos = eventAnimations.at(0).second.getPoint(eventAnimations.at(0).second.getSplineTime());
			currentVec = eventAnimations.at(0).second.getDerivative(eventAnimations.at(0).second.getSplineTime());
		}

		std::vector<std::pair<float, glm::vec3> > keyframes;

		keyframes = {};
		keyframes.emplace_back(0.f, currentPos);
		keyframes.emplace_back(endTransitionTime, idleLoop.getPoint(idleLoop.getSplineTime()));

		endTransition = std::make_unique<Spline>(keyframes, smoothing * currentVec, smoothing * -idleLoop.getDerivative(idleLoop.getSplineTime()));

		eventAnimations.clear();
	}
}