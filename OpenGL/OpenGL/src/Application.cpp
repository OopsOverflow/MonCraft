#include <iostream>
#include <memory>

#include "SDL2/SDL_image.h"

#include "gl/Shader.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"
#include "terrain/Terrain.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/Loader.hpp"
#include "util/Raycast.hpp"
#include "entity/character/Character.hpp"
#include "audio/Music.hpp"

#include "debug/Debug.hpp"
#include "debug/DebugBlock.hpp"

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    800

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";

    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    Terrain terrain;
    SkyBox sky;
    Character character({ 5000.0f,100.0f,5000.0f });
    ShadowMap shadows(1024);
    Loader loader;
    Raycast caster(100.f);
    std::unique_ptr<Mesh> targetBlock = makeDebugBlock();

    GLuint textureID = loader.loadTexture("Texture_atlas");

    Music MusicPlayer;

    int skyCamSize = 300;
    Camera skyCam(skyCamSize, skyCamSize, {1, 500, 1}, {0, 0, 0});

    glEnable(GL_SCISSOR_TEST);

    // main loop
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {

        // updates
        MusicPlayer.update();

        window.keyboardController.apply(character);
        window.mouseController.apply(character, terrain);
        character.update(terrain, dt);
        character.cameraToHead(window.camera);

        auto playerPos = window.camera.position;
        auto viewDir = window.camera.center - window.camera.position;
        auto fovX = glm::degrees(2 * atan(tan(glm::radians(45.f) / 2) * window.width / window.height)); // see https://en.wikipedia.org/wiki/Field_of_view_in_video_games#Field_of_view_calculations
        terrain.update(playerPos, viewDir, fovX);

        auto castPos = window.camera.position + .5f; // COMBAK: find out why camera pos is offset by .5 relative to block origin
        auto castDir = window.camera.center - window.camera.position;
        auto cast = caster.cast(castPos, castDir, terrain);
        targetBlock->model = glm::translate(glm::mat4(1.f), cast.position);

        // draw the shadow map
        // float t = timeBegin / 10000.f;
        float t = glm::half_pi<float>();
        float distance = 100.f;
        float a = cos(t);
        float b = sin(t);
        if(b < 0) b = -b;
        auto sunPos = cast.position + glm::normalize(glm::vec3(a, b, a)) * distance;
        auto sunTarget = cast.position;
        shadows.update(sunPos, sunTarget);
        shadows.beginFrame();
        terrain.render();
        shadows.endFrame();

        // prepare render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.activate();
        window.camera.activate();

        // set light position / intensity
        glUniform1f(shader.getUniformLocation("lightIntensity"), 1);
        auto sunDir = window.camera.view * glm::normalize(-glm::vec4(a, b, a, 0.f));
        glUniform3fv(shader.getUniformLocation("lightDirection"), 1, glm::value_ptr(sunDir));

        // bind textures
        GLint texSampler = shader.getUniformLocation("textureSampler");
        GLint shadowSampler = shader.getUniformLocation("shadowSampler");
        glUniform1i(texSampler, 0);
        glUniform1i(shadowSampler,  1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadows.getTextureID());

        // draw target block
        // glBindVertexArray(targetBlock->getVAO());
        // glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(targetBlock->model));
        // glDrawElements(GL_TRIANGLES, targetBlock->getVertexCount(), GL_UNSIGNED_INT, nullptr);
        // glBindVertexArray(0);

        // draw the terrain
        // shadows.activate();
        terrain.render();

        // terrain sky view
        glm::vec3 skyPos(window.camera.position.x, 500, window.camera.position.z);
        glm::vec3 skyCenter(window.camera.position.x, 0, window.camera.position.z - 1);
        skyCam.setLookAt(skyPos, skyCenter);
        skyCam.activate();

        glScissor(0, 0, skyCamSize + 5, skyCamSize + 5);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.render();
        glScissor(skyCamSize/2-1, skyCamSize/2-1, 2, 2);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT); // draw point

        // draw a dot in the middle of the screen
        float pointSize = 8;
        glScissor((window.width - pointSize) / 2, (window.height - pointSize) / 2, pointSize, pointSize);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT); // draw point
        glScissor(0, 0, window.width, window.height);

        // draw the character
        window.camera.activate();
        if (character.view == View::THIRD_PERSON)character.render();

        // draw skybox at last
        sky.render(window.camera);
    }

    return 0;
}
