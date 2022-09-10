#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Bezier.hpp"

static const float smoothing = 0.2f;
/* Spline must contain at least one Bezier or two Keyframes
*/
class Spline {
public:
    Spline(std::pair<uint32_t, Bezier> bezier);
    Spline(std::vector<std::pair<uint32_t, Bezier> > beziers);
    Spline(std::vector<std::pair<uint32_t, glm::vec3> > keyframes);
    Spline(std::vector<std::pair<uint32_t, glm::vec3> > keyframes, glm::vec3 initialVector, glm::vec3 finalVector);

    glm::vec3 compute(uint32_t &dt);

    void reset();

    uint32_t getTotalTime(){return totalTime;}
    uint32_t getSplineTime(){return splineTime;}
    
    glm::vec3 getCurrentPoint();
    glm::vec3 getCurrentDerivative();
    float getCurrentSpeed();
    
    glm::vec3 getPoint(uint32_t t);
    glm::vec3 getDerivative(uint32_t t);
    float getSpeed(uint32_t t);

    float getLength(int bezierPrecision);

private:
    //Duration, Bezier
    std::vector<std::pair<uint32_t, Bezier> > beziers;
    
    size_t currentBezier;
    uint32_t currentBezierTime;
    
    uint32_t splineTime;
    uint32_t totalTime;

    bool finished;
};