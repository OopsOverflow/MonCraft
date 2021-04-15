#pragma once


#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>


struct BodyPart {

    int nbVertices = 0;
    GLuint vbo;
    glm::mat4 propagatedMatrix = glm::mat4(1.0f);
    glm::mat4 localMatrix = glm::mat4(1.0f);
    std::vector<BodyPart*> children;
};


class Character {
public:
    Character(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3& headRotation = { 0.0f, 0.0f, 0.0f },
        const glm::vec2& bobyRotation = { 0.0f, 0.0f });


    void setPosition(const glm::vec3& position);
    void setLookAt(const glm::vec3& lookAt);

    void translate(const glm::vec3& direction, bool localSpace = false);

    void rotateHead(const glm::vec3& rotation, bool localSpace = false);
    void rotateBody(const glm::vec3& rotation, bool localSpace = false);

public:
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 center;

private:
    BodyPart ground;//No graphical display here, it's just for coordonates and height reference
    void computeProjection();
    void computeView();
};