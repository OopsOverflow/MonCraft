#include "SkyBox.hpp"
#include "gl/Camera.hpp"
#include "gl/Viewport.hpp"
#include "gl/ResourceManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint SkyBox::initSky(Shader& shader) {
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
    skyBoxShader("src/shader/skyBox.vert", "src/shader/skyBox.frag"),
    buffer(initSky(skyBoxShader))
{
  skyTxr = ResourceManager::getTexture("skybox");
}

void SkyBox::render(Camera& camera, float rotation)
{
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    skyBoxShader.activate();
    glBindVertexArray(buffer);
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    view = glm::rotate(view, rotation, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(MATRIX_VIEW, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(MATRIX_PROJECTION, 1, GL_FALSE, glm::value_ptr(camera.projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTxr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
