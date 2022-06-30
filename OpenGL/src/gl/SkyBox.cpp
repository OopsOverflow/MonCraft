#include "SkyBox.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stddef.h>
#include <memory>

#include "gl/Camera.hpp"
#include "gl/ResourceManager.hpp"
#include "gl/Shader.hpp"
#include "terrain/World.hpp"

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
    buffer(initSky()), blendFactor(0.f), rotation(0.f)
{
  skyDayTxr = ResourceManager::getTexture("skyboxDay");
  skyNightTxr = ResourceManager::getTexture("skyboxNight");

  skyBoxShader->activate();
  // Set Texture Locations for CubeSamplers.
  glUniform1i(skyBoxShader->getUniform("skyboxD"), 0);
  glUniform1i(skyBoxShader->getUniform("skyboxN"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyDayTxr);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyNightTxr);

}

void SkyBox::calcBlendFactor(uint32_t skytime) {
    
    uint32_t time = (uint32_t)(skytime * convertFactor);

    if (time >= 0 && time < 5000)
        blendFactor = 1.f;
    else if (time >= 5000 && time < 8000)
        blendFactor = (float)(8000 - time) / (float)(8000 - 5000);
    else if (time >= 8000 && time < 21000)
        blendFactor = 0.f;
    else 
        blendFactor = (float)(time - 21000) / (float)(24000 - 21000);

    glUniform1f(skyBoxShader->getUniform("blendFactor"), blendFactor);

}

void SkyBox::render(Camera& camera)
{
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    skyBoxShader->activate();
    glBindVertexArray(buffer);
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    rotation = fmod(rotation + World::getInst().dt * skyRotSpeed, glm::pi<float>() * 2.f);
    view = glm::rotate(view, rotation, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(skyBoxShader->getUniform(MATRIX_VIEW), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(skyBoxShader->getUniform(MATRIX_PROJECTION), 1, GL_FALSE, glm::value_ptr(camera.projection));

    // Sampling
    calcBlendFactor(World::getInst().t);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
