#pragma once


#include <GL/glew.h>
#include <GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stack>
#include <iostream>

#include "Shader.hpp"
#include "Camera.hpp"
#include "../util/Mesh.hpp"
#include "../util/Loader.hpp"

struct BodyPart {

    int vertexCount = 0;
    GLuint vao;
    glm::mat4 propagatedMatrix = glm::mat4(1.0f);
    glm::mat4 localMatrix = glm::mat4(1.0f);
    glm::vec2 localRotation = { 0.0f,0.0f };
    std::vector<BodyPart> children;
};

enum Member { LEFT_ARM = 1, RIGHT_ARM = 2, LEFT_LEG = 3, RIGHT_LEG = 4 };

class Character {
public:
    Character(const glm::vec3 headRotation, const glm::vec2 bobyRotation);


    void setView(const glm::vec3& view);
    void draw(glm::mat4 const& characterPos);
    void renderBody(const BodyPart& body, std::stack<glm::mat4>& mvpStack);

    void movement(const float distance);
    void rotateBody(float rotation);

    void rotateHead(glm::vec2 rotation);
    void rotateChest(float rotation);
    void rotateMember(glm::vec2 rotation, Member member);


private:
    Loader loader;
    GLuint texture;
    BodyPart chest;
    glm::vec3 view;
    float bodyOrientation;
};