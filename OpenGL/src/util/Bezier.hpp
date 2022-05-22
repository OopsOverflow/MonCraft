#pragma once

#include <glm/glm.hpp>

class Bezier {
public:    
    Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

    glm::vec3 calculatePoint(float t);
    glm::vec3 derivative(float t);

    float descreteLength(int nbSteps);

    glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;

};