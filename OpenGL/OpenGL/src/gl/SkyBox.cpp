#include "SkyBox.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stddef.h>
#include <memory>

#include "gl/Camera.hpp"
#include "gl/ResourceManager.hpp"
#include "gl/Shader.hpp"

GLuint SkyBox::initSky() {
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(skyboxVertices), skyboxVertices);

        glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(VERTEX_POSITION);
    }
    glBindVertexArray(0);

    return vao;
}

SkyBox::SkyBox() :
    skyBoxShader(ResourceManager::getShader("skyBox")),
    buffer(initSky())
{
  skyDayTxr = ResourceManager::getTexture("skyboxDay");
  skyNightTxr = ResourceManager::getTexture("skyboxNight");

  skyBoxShader->activate();
  // Set Texture Locations for CubeSamplers.
  glUniform1i(skyBoxShader->getUniform("skyboxD"), 0);
  glUniform1i(skyBoxShader->getUniform("skyboxN"), 1);
}

void SkyBox::calcBlendFactor(float skytime) {

    int time = (int)(skytime * 10000);
    // 24 Hour system
    time %= 24000;
    int texture1, texture2;

    float blendFactor;
    if (time >= 0 && time < 5000) {
        texture1 = skyNightTxr;
        texture2 = skyNightTxr;
        blendFactor = (float)(time - 0) / (float)(5000 - 0);
    }
    else if (time >= 5000 && time < 8000) {
        texture1 = skyNightTxr;
        texture2 = skyDayTxr;
        blendFactor = (float)(time - 5000) / (float)(8000 - 5000);
    }
    else if (time >= 8000 && time < 21000) {
        texture1 = skyDayTxr;
        texture2 = skyDayTxr;
        blendFactor = (float)(time - 8000) / (float)(21000 - 8000);
    }
    else {
        texture1 = skyDayTxr;
        texture2 = skyNightTxr;
        blendFactor = (float)(time - 21000) / (float)(24000 - 21000);
    }

    glUniform1f(skyBoxShader->getUniform("blendFactor"), blendFactor);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);

}

void SkyBox::render(Camera& camera, float time)
{
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    skyBoxShader->activate();
    glBindVertexArray(buffer);
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    time += 0.8f; // 8:00 (morning)
    view = glm::rotate(view, time, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(skyBoxShader->getUniform(MATRIX_VIEW), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(skyBoxShader->getUniform(MATRIX_PROJECTION), 1, GL_FALSE, glm::value_ptr(camera.projection));

    // Sampling
    calcBlendFactor(time);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
