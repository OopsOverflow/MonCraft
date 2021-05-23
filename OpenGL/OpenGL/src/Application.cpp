#include <iostream>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "SDL2/SDL_image.h"

#include "gl/Shader.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"
#include "terrain/Terrain.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/TextureLoader.hpp"
#include "gl/Font.hpp"
#include "gl/ResourceManager.hpp"
#include "util/Raycast.hpp"
#include "entity/character/Character.hpp"
#include "audio/Music.hpp"
#include "ui/ui.hpp"

#include "debug/Debug.hpp"

#include <functional>
using namespace glm;

// WINDOW DIMENSIONS
#define WIDTH     800
#define HEIGHT    800

void loadResources() {
  ResourceManager::addShader("simple", "src/shader/simple.vert", "src/shader/simple.frag");
  ResourceManager::addShader("font",   "src/shader/font.vert",   "src/shader/font.frag");
  ResourceManager::addShader("pane",   "src/shader/pane.vert",   "src/shader/pane.frag");
}

int main(int argc, char* argv[]) {
    std::cout << "----Main------\n";

    // game seed
    std::hash<std::string> hashString;
    std::srand(hashString("Moncraft"));


    Viewport window(WIDTH, HEIGHT);
    loadResources();
    Shader* shader = ResourceManager::getShader("simple");

    Terrain terrain;
    SkyBox sky;

    ShadowMap shadows(4096);
    Character character({ 0.0f, 40.0f, 0.0f });

    // UI stuff
    auto font = std::make_shared<const Font>("Roboto-Regular");
    // auto font = std::make_shared<const Font>("VT323-Regular");
    ui::Root interface({ WIDTH, HEIGHT });
    ui::Pane pane(&interface);
    pane.setPosition({ 10, 10 });
    pane.setAnchorX(ui::Anchor::CENTER);
    pane.setColor({ 1.f, 1.f, 1.f, 1.f });
    pane.setPadding({ 10, 10 });
    ui::Text text_fps(&pane, "hello", font);
    text_fps.setFontSize(2.f);
    // text_fps.setAnchorX(ui::Anchor::CENTER);
    // text_fps.setAnchorY(ui::Anchor::END);
    text_fps.setColor({ 0.8f, 0.7f, 0.0f, 0.5f });

    TextureLoader loader;
    Raycast caster(100.f);

    std::cout << "Chargement des Textures";
    GLuint textureID = loader.loadTexture("Texture_atlas");
    GLuint normalMapID[30];
    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        normalMapID[i] = loader.loadTexture(filename);
    }
    std::cout << " --Termine" << std::endl;

    Music MusicPlayer;

    float t = 0;

    // main loop
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {

        t += dt;

        // updates
        MusicPlayer.update();

        window.keyboardController.apply(character, terrain);
        window.mouseController.apply(character, terrain);
        character.update(terrain, dt);
        character.cameraToHead(window.camera);

        auto playerPos = window.camera.position;
        auto viewDir = window.camera.center - window.camera.position;
        terrain.update(playerPos, viewDir, window.camera.getFovX());

        // draw the shadow map

        float sunSpeed = 5.f;
        float sunTime = pi<float>() * .25f;
        sunTime += t / 300.f * sunSpeed;
        glUniform1f(shader->getUniformLocation("sunTime"), sunTime * 400); // Fog Sampling Time
        // This is used instead to avoid Win/Linux conflicts

        float distance = 100.f;
        auto sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * distance;
        shadows.update(sunDir);

        shadows.attach(window.camera, Frustum::NEAR);
        shadows.beginFrame(Frustum::NEAR);
        terrain.render(shadows.camera);
        character.render();
        shadows.endFrame();

        shadows.attach(window.camera, Frustum::MEDIUM);
        shadows.beginFrame(Frustum::MEDIUM);
        terrain.render(shadows.camera);
        shadows.endFrame();

        shadows.attach(window.camera, Frustum::FAR);
        shadows.beginFrame(Frustum::FAR);
        terrain.render(shadows.camera);
        shadows.endFrame();

        // prepare render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw skybox at last
        window.camera.activate();
        sky.render(window.camera);

        shader->activate();

        // set light position / intensity
        glUniform1f(shader->getUniformLocation("lightIntensity"), 1);
        auto sunDirViewSpace = window.camera.view * vec4(sunDir, 0.0);
        glUniform3fv(shader->getUniformLocation("lightDirection"), 1, value_ptr(sunDirViewSpace));

        // bind textures
        GLint texSampler = shader->getUniformLocation("textureSampler");
        glUniform1i(texSampler, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLint normalMap = shader->getUniformLocation("normalMap");
        glUniform1i(normalMap, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMapID[(size_t)(t*15)%30]);


        Block* block = terrain.getBlock(ivec3(window.camera.position + vec3(-0.5f,0.6f,-0.5f)));
        if (block) {
            bool isUnderWater = block->type == BlockType::Water;
            GLint underWater = shader->getUniformLocation("underWater");
            glUniform1i(underWater, isUnderWater);

            sky.skyBoxShader.activate();
            underWater = sky.skyBoxShader.getUniformLocation("underWater");
            glUniform1i(underWater, isUnderWater);

            shader->activate();
        }

        GLint fog = shader->getUniformLocation("fog");
        glUniform1i(fog, (int)window.enableFog);

        // draw the terrain
        window.camera.activate();
        shadows.activate();
        terrain.render(window.camera);

        // dot in the middle of the screen
        glEnable(GL_SCISSOR_TEST);
        {
          float pointSize = 8;
          glScissor((window.width - pointSize) / 2, (window.height - pointSize) / 2, pointSize, pointSize);
          glClearColor(1, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT); // draw point
        }
        glDisable(GL_SCISSOR_TEST);

        // draw the character
        window.camera.activate();
        if (character.view == View::THIRD_PERSON) character.render();

        // draw / update ui
        interface.setSize({ window.width, window.height });
        std::string text = "FPS : " + std::to_string((int)(1.f / dt));
        text_fps.setText(text);
        interface.render();
    }

    return 0;
}
