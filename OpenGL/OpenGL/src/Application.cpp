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

using namespace glm;

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    800

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";

    Viewport window(WIDTH, HEIGHT);
    Shader shader("src/shader/simple.vert", "src/shader/simple.frag");
    Terrain terrain;
    SkyBox sky;
    Character character({ 0.0f,100.0f,0.0f });
    ShadowMap shadows(4096);
    Loader loader;
    Raycast caster(100.f);

    GLuint textureID = loader.loadTexture("Texture_atlas");

    Music MusicPlayer;

    int skyCamSize = 300;
    Camera skyCam(skyCamSize, skyCamSize, {1, 500, 1}, {0, 0, 0});

    float t = 0;

    // main loop
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {

        t += dt;

        // updates
        MusicPlayer.update();

        window.keyboardController.apply(character);
        window.mouseController.apply(character, terrain);
        character.update(terrain, dt);
        character.cameraToHead(window.camera);

        auto playerPos = window.camera.position;
        auto viewDir = window.camera.center - window.camera.position;
        terrain.update(playerPos, viewDir, window.camera.getFovX());

        // draw the shadow map
        float sunSpeed = 1.f;
        float sunTime = pi<float>() * .25f;
        sunTime += t / 1000.f * sunSpeed;
        float distance = 100.f;
        float a = cos(sunTime);
        float b = sin(sunTime);
        if(b < 0) b = -b;
        auto sunPos = normalize(vec3(a, b, a)) * distance;
        auto sunTarget = vec3(0);
        shadows.update(sunPos, sunTarget);
        shadows.attach(window.camera);

        shadows.beginFrame(Frustum::NEAR);
        terrain.render();
        character.render();
        shadows.endFrame();
        shadows.beginFrame(Frustum::MEDIUM);
        terrain.render();
        character.render();
        shadows.endFrame();
        shadows.beginFrame(Frustum::FAR);
        terrain.render();
        shadows.endFrame();

        // prepare render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.activate();

        // set light position / intensity
        glUniform1f(shader.getUniformLocation("lightIntensity"), 1);
        auto sunDir = window.camera.view * normalize(-vec4(a, b, a, 0.f));
        glUniform3fv(shader.getUniformLocation("lightDirection"), 1, value_ptr(sunDir));

        // bind textures
        GLint texSampler = shader.getUniformLocation("textureSampler");
        glUniform1i(texSampler, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // draw the terrain
        window.camera.activate();
        shadows.activate(shader);
        terrain.render();

        // terrain sky view
        glEnable(GL_SCISSOR_TEST);
        {
          glScissor(0, 0, skyCamSize + 5, skyCamSize + 5);
          glClearColor(0, 0, 0, 0);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          // draw shadow view
          // shadows.camera.activate();
          glUniformMatrix4fv(MATRIX_VIEW, 1, GL_FALSE, value_ptr(mat4(1.f)));
          // glUniformMatrix4fv(MATRIX_NORMAL, 1, GL_FALSE, value_ptr());
          glUniformMatrix4fv(MATRIX_PROJECTION, 1, GL_FALSE, value_ptr(shadows.shadowMatrices[1]));
          glViewport(0, 0, skyCamSize, skyCamSize),
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
        }
        glDisable(GL_SCISSOR_TEST);

        // draw the character
        window.camera.activate();
        if (character.view == View::THIRD_PERSON) character.render();

        // draw skybox at last
        sky.render(window.camera);
    }

    return 0;
}
