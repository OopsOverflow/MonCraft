#include "SkyBox.hpp"
#include "../gl/Camera.hpp"
#include "../gl/Viewport.hpp"
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

// Easy swap SkyBox
// Tool : https://jaxry.github.io/panorama-to-cubemap/
GLuint SkyBox::loadTxr() {
    Loader skyTxr;
    std::vector<std::string> faces
    {
        "skybox/px.png",            // right
        "skybox/nx.png",            // left
        "skybox/py.png",            // top
        "skybox/ny.png",            // bottom
        "skybox/pz.png",            // front
        "skybox/nz.png"             // back
    };

    return skyTxr.loadCubeMap(faces);
}

SkyBox::SkyBox() :
    skyBoxShader("src/shader/skyBox.vert", "src/shader/skyBox.frag"),
    buffer(initSky(skyBoxShader)),
    skyTxr(loadTxr())
{

}

void SkyBox::render(Camera& camera)
{
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    skyBoxShader.activate();
    glBindVertexArray(buffer);
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    glUniformMatrix4fv(MATRIX_VIEW, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(MATRIX_PROJECTION, 1, GL_FALSE, glm::value_ptr(camera.projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTxr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
