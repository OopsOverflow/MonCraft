#include "Bezier.hpp"

Bezier::Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) :
    p0(p0), p1(p1), p2(p2), p3(p3)
{
}


glm::vec3 Bezier::calculatePoint(float t) {
    float oneMinusT = 1 - t;

    return p0 * (oneMinusT * oneMinusT * oneMinusT) + 
    p1 * (3.f * t * oneMinusT * oneMinusT) + 
    p2 * (3.f * t * t * oneMinusT) + 
    p3 * (t * t * t);  

}

glm::vec3 Bezier::derivative(float t) {
    float TMinusOne = t - 1;

    // false but works anyway
    // return p0 * (- TMinusOne * TMinusOne) + 
    // p1 * (3.f * (- 1.f/ 3.f + t) * TMinusOne) + 
    // p2 * ((2.f - 3.f * t) * t) + 
    // p3 * (t * t);  

    // real derivative
    return p0 * (- 3.f * TMinusOne * TMinusOne) + 
    p1 * ( (- 3.f + 9.f * t) * TMinusOne) + 
    p2 * ((6.f - 9.f * t) * t) + 
    p3 * ( 3.f * t * t);  

}

float Bezier::descreteLength(int nbSteps) {
    float step = 1.f / nbSteps;
    float threshold = step / 2.f;
    float length = 0.f;
    for(float i = 0.f; abs(1.f - i) >  threshold; i += step) {
        length += glm::distance(calculatePoint(i + step), calculatePoint(i));

    }

    return length;

}