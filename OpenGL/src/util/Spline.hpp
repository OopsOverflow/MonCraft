#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Bezier.hpp"

static const float smoothing = 0.2f;
/* Spline must contain at least one Bezier or two Keyframes
*/
class Spline {
public:
    Spline(std::pair<float, Bezier> bezier);
    Spline(std::vector<std::pair<float, Bezier> > beziers);
    Spline(std::vector<std::pair<float, glm::vec3> > keyframes);
    Spline(std::vector<std::pair<float, glm::vec3> > keyframes, glm::vec3 initialVector, glm::vec3 finalVector);

    glm::vec3 compute(float &dt);

    void add(std::vector<std::pair<float, Bezier>> beziers);
    void add(std::pair<float, Bezier> bezier);

    void reset();

    float getTotalTime(){return totalTime;}
    float getSplineTime(){return splineTime;}

    glm::vec3 getDerivative(float t);
    glm::vec3 getPoint(float t);

    float getLength(int bezierPrecision);

private:
    //Duration, Bezier
    std::vector<std::pair<float, Bezier> > beziers;
    float splineTime;
    float totalTime;

    bool finished;
};