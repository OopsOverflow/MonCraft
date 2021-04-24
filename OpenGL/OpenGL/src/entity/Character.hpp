#pragma once


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stack>
#include <iostream>

#include "Shader.hpp"
#include "../util/Mesh.hpp"
#include "../util/Loader.hpp"


struct BodyPart {

    int vertexCount = 0;
    GLuint vao;
    glm::mat4 propagatedMatrix = glm::mat4(1.0f);
    glm::mat4 localMatrix = glm::mat4(1.0f);
    glm::vec3 localRotation = { 0.0f,0.0f,0.0f };
    glm::vec3 localPosition = { 0.0f,0.0f,0.0f };
    glm::vec3 reachRotation = { 0.0f,0.0f,0.0f };

    std::vector<BodyPart> children;
};

enum Member { LEFT_ARM = 1, RIGHT_ARM = 2, LEFT_LEG = 3, RIGHT_LEG = 4 };

class Character {
public:
    Character(const glm::vec3 headRotation, const glm::vec2 bobyRotation);

    void setView(const glm::vec3& view);
    void draw(glm::mat4 const& characterPos, bool onlyRightHand);
    void renderBody(const BodyPart& body, std::stack<glm::mat4>& mvpStack);

    void rotateBody(glm::vec3 rotation);
    BodyPart const& getHeadProperties();
    glm::vec3 getBodyRotation();
    void rotateHead(glm::vec2 rotation);
    void setMemberRotation(glm::vec2 rotation, Member member);

    void headReachRotation(float maxRotation);
    void bodyReachRotation(float maxRotation);
    void memberReachRotation(float maxRotation, Member member);


private:
    Loader loader;
    GLuint texture;
    BodyPart chest;

};
