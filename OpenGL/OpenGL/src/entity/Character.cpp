#include "Character.hpp"

Character::Character(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3& headRotation = { 0.0f, 0.0f, 0.0f },
    const glm::vec2& bobyRotation = { 0.0f, 0.0f }){

    static const int cubeVertices = 36;
    GLfloat cube[] = {
    //Top
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

    //Bottom
        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

    //Front
       -0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

    //Right
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,

    //Back
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,

    //Left
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f

    };

    BodyPart head;
    head.nbVertices = cubeVertices;
    head.vbo = genVBO(cube);

    BodyPart chest;
    chest.nbVertices = cubeVertices;
    chest.vbo = genVBO(cube);
    chest.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 0.5f));
    chest.propagatedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.25f, 0.0f));

    BodyPart leftArm;
    leftArm.nbVertices = cubeVertices;
    leftArm.vbo = genVBO(cube);
    leftArm.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.5f, 0.5f));
    leftArm.propagatedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, 0.0f, 0.0f));

    BodyPart rightArm;
    rightArm.nbVertices = cubeVertices;
    rightArm.vbo = genVBO(cube);
    rightArm.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.5f, 0.5f));
    rightArm.propagatedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.0f, 0.0f));

    BodyPart leftLeg;
    leftLeg.nbVertices = cubeVertices;
    leftLeg.vbo = genVBO(cube);
    leftLeg.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.5f, 0.5f));
    leftLeg.propagatedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -1.25f, 0.0f));

    BodyPart rightLeg;
    rightLeg.nbVertices = cubeVertices;
    rightLeg.vbo = genVBO(cube);
    rightLeg.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 1.5f, 0.5f));
    rightLeg.propagatedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -1.25f, 0.0f));

    this->ground.children.push_back(&head);

    head.children.push_back(&chest);

    chest.children.push_back(&rightArm);
    chest.children.push_back(&leftArm);
    chest.children.push_back(&rightLeg);
    chest.children.push_back(&leftLeg);

}


void Character::setPosition(const glm::vec3& position) {

}
void Character::setLookAt(const glm::vec3& lookAt) {

}

void Character::translate(const glm::vec3& direction, bool localSpace = false) {

}

void Character::rotateHead(const glm::vec3& rotation, bool localSpace = false) {

}
void Character::rotateBody(const glm::vec3& rotation, bool localSpace = false) {


}