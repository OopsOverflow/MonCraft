#include "Spline.hpp"

#include <stdexcept>
#include <iostream>



Spline::Spline(std::vector<std::pair<float, Bezier> > beziers) : 
    beziers(beziers), splineTime(0.f), finished(false), currentBezier(0), currentBezierTime(0.f) 
{
    totalTime = 0.f;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        totalTime += beziers.at(i).first;
    }
}

Spline::Spline(std::vector<std::pair<float, glm::vec3> > keyframes) :  Spline(keyframes,
                                                                              -(keyframes[keyframes.size() - 2].second - keyframes[1].second) * smoothing,
                                                                               (keyframes[keyframes.size() - 2].second - keyframes[1].second) * smoothing) {

}

Spline::Spline(std::pair<float, Bezier> bezier) : Spline(std::vector<std::pair<float, Bezier> >({bezier})) {
    
}

Spline::Spline(std::vector<std::pair<float, glm::vec3> > keyframes, glm::vec3 initialVector, glm::vec3 finalVector) : 
    splineTime(0.f), finished(false), currentBezier(0), currentBezierTime(0.f)
{
    std::vector<std::pair<float, Bezier> > res;

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
    totalTime = 0.f;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        totalTime += beziers.at(i).first;
    }
}

glm::vec3 Spline::compute(float &dt) {
    if(!finished)
    {
        while(dt > 0.f) { 
            float currentBezierTimeRemain = beziers.at(currentBezier).first - currentBezierTime;
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
                dt = 0.f;
            }
        }
        return beziers.at(currentBezier).second.calculatePoint(currentBezierTime / beziers.at(currentBezier).first);
    }
    return beziers.at(beziers.size()-1).second.p3;

}

//Deprecated
// void Spline::add(std::vector<std::pair<float, Bezier>> beziers) {
//     this->beziers.insert(this->beziers.end(), beziers.begin(), beziers.end());
// }

// void Spline::add(std::pair<float, Bezier> bezier) {
//     this->beziers.push_back(bezier);
// }

void Spline::reset() {
    finished = false;
    splineTime = 0.f;
    currentBezier = 0;
    currentBezierTime = 0.f;
}

glm::vec3 Spline::getPoint(float t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0.f) {
        t -= beziers.at(currentBezier).first;
        if(t > 0.f) currentBezier += 1;
    }

    if(t < 0.f)t += beziers.at(currentBezier).first;
    return beziers.at(currentBezier).second.calculatePoint(t / beziers.at(currentBezier).first);

}

glm::vec3 Spline::getDerivative(float t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0.f) {
        t -= beziers.at(currentBezier).first;
        if(t > 0.f) currentBezier += 1;
    }

    if(t < 0)t += beziers.at(currentBezier).first;

    return beziers.at(currentBezier).second.derivative(t / beziers.at(currentBezier).first);

}

float Spline::getSpeed(float t) {
    if(t > totalTime) t = totalTime;
    
    size_t currentBezier = 0;
    while(t >= 0.f) {
        t -= beziers.at(currentBezier).first;
        if(t > 0.f) currentBezier += 1;
    }

    if(t < 0)t += beziers.at(currentBezier).first;

    return 1.f/beziers.at(currentBezier).first;

}

glm::vec3 Spline::getCurrentPoint() {
    if(!finished)
        return beziers.at(currentBezier).second.calculatePoint(currentBezierTime / beziers.at(currentBezier).first);

    return beziers.at(beziers.size() - 1).second.calculatePoint(1.f);

}

glm::vec3 Spline::getCurrentDerivative() {
    if(!finished)
        return beziers.at(currentBezier).second.derivative(currentBezierTime / beziers.at(currentBezier).first);

    return beziers.at(beziers.size() - 1).second.derivative(1.f);
}

float Spline::getCurrentSpeed() {
    if(!finished) {
        return 1.f / beziers.at(currentBezier).first;
    }
        
    return 1.f / beziers.at(beziers.size() - 1).first;
}


float Spline::getLength(int bezierPrecision) {
    float length = 0.f;
    for(size_t i = 0; i < beziers.size(); i += 1) {
        length += beziers.at(i).second.descreteLength(bezierPrecision);

    }

    return length;

}