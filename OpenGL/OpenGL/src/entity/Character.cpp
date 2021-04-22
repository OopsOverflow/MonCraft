#include "Character.hpp"
#include "../util/Mesh.hpp"
#include "Cube.hpp"

Character::Character(const glm::vec3 headRotation, const glm::vec2 bobyRotation)
    : view({ 0.0f,0.0f,0.0f }), bodyRotation({ 0.0f,0.0f,0.0f }), loader(), texture(loader.loadTexture("Character"))
{
    Cube cube;
    Mesh* chestMesh = new Mesh(cube.vertices, cube.normals, cube.chestUVs, cube.occlusions, cube.indices);

    chest.vertexCount = chestMesh->getVertexCount();
    chest.vao = chestMesh->getVAO();
    chest.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(8, 12, 4));
    chest.propagatedMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.069375f));

    Mesh* headMesh = new Mesh(cube.vertices, cube.normals, cube.headUVs, cube.occlusions, cube.indices);
    BodyPart head;
    head.vertexCount = headMesh->getVertexCount();
    head.vao = headMesh->getVAO();
    head.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(8, 8, 8));
    head.localPosition = glm::vec3(0.0f, (12 / 2 + 8 / 2), 0.0f);
    head.propagatedMatrix = glm::translate(glm::mat4(1.0f), head.localPosition);
    head.localPosition *= 0.069375f;
    chest.children.push_back(head);

    Mesh* leftArmMesh = new Mesh(cube.vertices, cube.normals, cube.leftArmUVs, cube.occlusions, cube.indices);
    BodyPart leftArm;
    leftArm.vertexCount = leftArmMesh->getVertexCount();
    leftArm.vao = leftArmMesh->getVAO();
    leftArm.localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4, 12, 4));
    leftArm.localPosition = glm::vec3((8 / 2 + 4 / 2), 0.0f, 0.0f);
    leftArm.propagatedMatrix = glm::translate(glm::mat4(1.0f), leftArm.localPosition);
    leftArm.localPosition *= 0.069375f;
    chest.children.push_back(leftArm);

    Mesh* rightArmMesh = new Mesh(cube.vertices, cube.normals, cube.rightArmUVs, cube.occlusions, cube.indices);
    BodyPart rightArm;
    rightArm.vertexCount = rightArmMesh->getVertexCount();
    rightArm.vao = rightArmMesh->getVAO();
    rightArm.localMatrix = leftArm.localMatrix;
    rightArm.localPosition = glm::vec3(-(8 / 2 + 4 / 2), 0.0f, 0.0f);
    rightArm.propagatedMatrix = glm::translate(glm::mat4(1.0f), rightArm.localPosition);
    rightArm.localPosition *= 0.069375f;
    chest.children.push_back(rightArm);

    Mesh* leftLegMesh = new Mesh(cube.vertices, cube.normals, cube.leftLegUVs, cube.occlusions, cube.indices);
    BodyPart leftLeg;
    leftLeg.vertexCount = leftLegMesh->getVertexCount();
    leftLeg.vao = leftLegMesh->getVAO();
    leftLeg.localMatrix = leftArm.localMatrix;
    leftLeg.localPosition = glm::vec3((4 / 2) * (88 / 91.0f), -(12 / 2 + 12 / 2), -(2 / 91.0f));//slightly shift legs to prevent Z-fighting
    leftLeg.propagatedMatrix = glm::translate(glm::mat4(1.0f), leftLeg.localPosition);
    leftLeg.localPosition *= 0.069375f;
    chest.children.push_back(leftLeg);

    Mesh* rightLegMesh = new Mesh(cube.vertices, cube.normals, cube.rightLegUVs, cube.occlusions, cube.indices);
    BodyPart rightLeg;
    rightLeg.vertexCount = rightLegMesh->getVertexCount();
    rightLeg.vao = rightLegMesh->getVAO();
    rightLeg.localMatrix = leftArm.localMatrix;
    rightLeg.localPosition = glm::vec3(-(4 / 2) * (88 / 91.0f), -(12 / 2 + 12 / 2), -(2 / 91.0f));
    rightLeg.propagatedMatrix = glm::translate(glm::mat4(1.0f), rightLeg.localPosition);
    rightLeg.localPosition *= 0.069375f;
    chest.children.push_back(rightLeg);

    meshes = { chestMesh, headMesh, leftArmMesh, rightArmMesh, leftLegMesh, rightLegMesh };
}

BodyPart const& Character::getHeadProperties() {
    return chest.children.at(0);
}

Character::~Character() {
  for(Mesh* mesh : meshes) {
    delete mesh;
  }
}

void setView(const glm::vec3& lookAt) {

}

void Character::draw(glm::mat4 const& characterPos, bool onlyRightHand) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (onlyRightHand) {

    }
    else {
        std::stack<glm::mat4> mvpStack;
        mvpStack.push(characterPos);
        this->renderBody(chest, mvpStack);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Character::renderBody(const BodyPart& body, std::stack<glm::mat4>& mvpStack) {
    glBindVertexArray(body.vao);
    glm::mat4 model = mvpStack.top() * body.propagatedMatrix * body.localMatrix;
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, body.vertexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    mvpStack.push(mvpStack.top() * body.propagatedMatrix);

    for (BodyPart child : body.children) {
         renderBody(child, mvpStack);
    }
    mvpStack.pop();
}

void Character::movement(const float distance) {

}

void Character::rotateBody(float rotation) {
    bodyRotation.y += rotation;
    BodyPart* model = &chest;
    model->propagatedMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.069375f));
    model->propagatedMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(bodyRotation.y), glm::vec3({ 0.0f,1.0f,0.0f })) * model->propagatedMatrix;
}

void Character::rotateHead(glm::vec2 rotation) {

    BodyPart* model = &chest.children.at(0);

   if (fabs(model->localRotation.y + rotation.y) > 45) {
        rotateBody(model->localRotation.y + rotation.y - (rotation.y > 0 ? 35 : -35));
       rotation.y = (model->localRotation.y + rotation.y > 0 ? 35 : -35) - model->localRotation.y;
    }
    if(fabs(model->localRotation.x + rotation.x) > 85) rotation.x = (model->localRotation.x + rotation.x > 0 ? 85 : -85) - model->localRotation.x;

    model->localRotation += glm::vec3({rotation.x, rotation.y, 0.0f});


    glm::vec3 pivotLocation;
    pivotLocation = glm::vec3(0.0f, -4, 0.0f);
    model->localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(8, 8, 8));

    model->localMatrix = glm::translate(glm::mat4(1.0f), -pivotLocation) * model->localMatrix;
    model->localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-model->localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))* model->localMatrix;
    model->localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(model->localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))* model->localMatrix;
    model->localMatrix = glm::translate(glm::mat4(1.0f), pivotLocation)* model->localMatrix;
    glm::vec4 newPos({ 0.0f,0.0f,0.0f,1.0f });
    newPos = chest.propagatedMatrix * model->propagatedMatrix * model->localMatrix * newPos;
    model->localPosition = glm::vec3(newPos);

    glm::vec3 rot =bodyRotation + model->localRotation;
}

void Character::rotateChest(float rotation) {
    BodyPart* model = &chest;
    model->propagatedMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(cos(glm::radians(bodyRotation.y)), 0.0f, -sin(glm::radians(bodyRotation.y)))) * model->propagatedMatrix;
}

void Character::rotateMember(glm::vec2 rotation, Member member) {
    BodyPart& model = chest.children.at((int)member);

    glm::vec3 pivotLocation;
    rotation.x = -rotation.x;   //angle is positive is the arm is extended forward

    if ((int)member <= 2) {
        pivotLocation = glm::vec3(0.0f, (2 * 12 / 6) * 0.069375f, 0.0f);
        if (member == Member::RIGHT_ARM) rotation.y = -rotation.y;  //the greater the angle, the further the arm is from the body
    }
    else {
        pivotLocation = glm::vec3(0.0f, (3 * 12 / 6) * 0.069375f, 0.0f);
        if (member == Member::RIGHT_LEG) rotation.y = -rotation.y;  //the greater the angle, the further the leg is from the body
    }

    model.localMatrix = glm::translate(glm::mat4(1.0f), -pivotLocation) * model.localMatrix;
    model.localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x) , glm::vec3(1.0f, 0.0f, 0.0f)) * model.localMatrix;
    model.localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y) , glm::vec3(0.0f, -sin(glm::radians(rotation.x)), cos(glm::radians(rotation.x)))) * model.localMatrix;
    model.localMatrix = glm::translate(glm::mat4(1.0f), pivotLocation) * model.localMatrix;
}
