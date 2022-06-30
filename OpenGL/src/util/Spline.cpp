#include "Spline.hpp"

#include <stdexcept>
#include <iostream>



Spline::Spline(std::vector<std::pair<uint32_t, Bezier> > beziers) : 
    beziers(beziers), splineTime(0), finished(false), currentBezier(0), currentBezierTime(0) 
{
    totalTime = 0;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        totalTime += beziers.at(i).first;
    }
}

Spline::Spline(std::vector<std::pair<uint32_t, glm::vec3> > keyframes) :  Spline(keyframes,
                                                                              -(keyframes[keyframes.size() - 2].second - keyframes[1].second) * smoothing,
                                                                               (keyframes[keyframes.size() - 2].second - keyframes[1].second) * smoothing) {

}

Spline::Spline(std::pair<uint32_t, Bezier> bezier) : Spline(std::vector<std::pair<uint32_t, Bezier> >({bezier})) {
    
}

Spline::Spline(std::vector<std::pair<uint32_t, glm::vec3> > keyframes, glm::vec3 initialVector, glm::vec3 finalVector) : 
    splineTime(0), finished(false), currentBezier(0), currentBezierTime(0)
{
    std::vector<std::pair<uint32_t, Bezier> > res;

    glm::vec3 previousVector = initialVector;
    for(size_t i = 1; i < keyframes.size(); i += 1) {
        if(i != keyframes.size() - 1) {
            glm::vec3 endVector;
            if(keyframes.at(i - 1).second == keyframes.at(i + 1).second || distance(keyframes.at(i - 1).second, keyframes.at(i + 1).second) < 0.0001f) {
                glm::vec3 tmp = cross(keyframes.at(i).second - keyframes.at(i - 1).second, -previousVector);
                endVector = normalize(cross(keyframes.at(i).second - keyframes.at(i - 1).second, tmp));
            }
            else {
                endVector = keyframes.at(i - 1).second - keyframes.at(i + 1).second;
            }

            endVector *= smoothing;

            Bezier newBezier(keyframes.at(i - 1).second, keyframes.at(i - 1).second + previousVector, keyframes.at(i).second + endVector, keyframes.at(i).second);
            res.emplace_back(keyframes.at(i).first - keyframes.at(i - 1).first, newBezier);

            previousVector = -endVector;

        }
        else {
            Bezier newBezier(keyframes.at(i - 1).second, keyframes.at(i - 1).second + previousVector, keyframes.at(i).second + finalVector, keyframes.at(i).second);
            res.emplace_back(keyframes.at(i).first - keyframes.at(i - 1).first, newBezier);

        }
    }

    beziers = res;
    totalTime = 0;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        totalTime += beziers.at(i).first;
    }
}

glm::vec3 Spline::compute(uint32_t &dt) {
    if(!finished)
    {
        while(dt > 0) { 
            uint32_t currentBezierTimeRemain = beziers.at(currentBezier).first - currentBezierTime;
            if(dt >= currentBezierTimeRemain) {
                splineTime += currentBezierTimeRemain;
                dt -= currentBezierTimeRemain;
                currentBezier += 1;
                if(currentBezier >= beziers.size()) {
                    finished = true;
                    splineTime = totalTime;
                    return beziers.at(beziers.size()-1).second.p3;
                }
                currentBezierTime = 0;
            }  
            else {
                currentBezierTime += dt;
                splineTime += dt;
                dt = 0;
            }
        }
        return beziers.at(currentBezier).second.calculatePoint(currentBezierTime / (float)beziers.at(currentBezier).first);
    }
    return beziers.at(beziers.size()-1).second.p3;

}

void Spline::reset() {
    finished = false;
    splineTime = 0;
    currentBezier = 0;
    currentBezierTime = 0;
}

glm::vec3 Spline::getPoint(uint32_t t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0) {
        t -= beziers.at(currentBezier).first;
        if(t > 0) currentBezier += 1;
    }

    if(t < 0)t += beziers.at(currentBezier).first;
    return beziers.at(currentBezier).second.calculatePoint(t / (float)beziers.at(currentBezier).first);

}

glm::vec3 Spline::getDerivative(uint32_t t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0) {
        t -= beziers.at(currentBezier).first;
        if(t > 0) currentBezier += 1;
    }

    if(t < 0)t += beziers.at(currentBezier).first;

    return beziers.at(currentBezier).second.derivative(t / (float)beziers.at(currentBezier).first);

}

float Spline::getSpeed(uint32_t t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0) {
        t -= beziers.at(currentBezier).first;
        if(t > 0) currentBezier += 1;
    }

    if(t < 0)t += beziers.at(currentBezier).first;

    return 1000.f/beziers.at(currentBezier).first;

}

glm::vec3 Spline::getCurrentPoint() {
    if(!finished)
        return beziers.at(currentBezier).second.calculatePoint(currentBezierTime / (float)beziers.at(currentBezier).first);

    return beziers.at(beziers.size() - 1).second.calculatePoint(1.f);

}

glm::vec3 Spline::getCurrentDerivative() {
    if(!finished)
        return beziers.at(currentBezier).second.derivative(currentBezierTime / (float)beziers.at(currentBezier).first);

    return beziers.at(beziers.size() - 1).second.derivative(1.f);
}

float Spline::getCurrentSpeed() {
    if(!finished) {
        return 1000.f / beziers.at(currentBezier).first;
    }
        
    return 1000.f / beziers.at(beziers.size() - 1).first;
}


float Spline::getLength(int bezierPrecision) {
    float length = 0.f;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        length += beziers.at(i).second.descreteLength(bezierPrecision);

    }
    return length;

}