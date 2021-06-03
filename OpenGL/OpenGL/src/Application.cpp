#include <iostream>
#include <memory>
#include <glm/gtc/type_ptr.hpp>

#include "gl/Shader.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"
#include "terrain/Terrain.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/Font.hpp"
#include "gl/ResourceManager.hpp"
#include "util/Raycast.hpp"
#include "entity/character/Character.hpp"
#include "audio/Music.hpp"
#include "ui/ui.hpp"
#include "ui/style/Style.hpp"

#include "debug/Debug.hpp"

#include <functional>
using namespace glm;

void loadResources() {
    // Easy swap SkyBox
    // Tool : https://jaxry.github.io/panorama-to-cubemap/
    static const std::vector<std::string> skyboxFaces {
        "skybox/px.png",  // right
        "skybox/nx.png",  // left
        "skybox/py.png",  // top
        "skybox/ny.png",  // bottom
        "skybox/pz.png",  // front
        "skybox/nz.png"   // back
    };

    ResourceManager::loadTexture("atlas", "Texture_atlas");
    ResourceManager::loadTexture("character", "Character");
    ResourceManager::loadShader("simple", "simple.vert", "simple.frag");
    ResourceManager::loadShader("font",   "font.vert",   "font.frag");
    ResourceManager::loadShader("pane",   "pane.vert",   "pane.frag");
    ResourceManager::loadCubeMap("skybox", skyboxFaces);

    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        ResourceManager::loadTexture("waterNormal" + std::to_string(i), filename);
    }
}

void drawMiddleDot(Viewport const& vp) {
    glEnable(GL_SCISSOR_TEST);
    {
        float pointSize = 8;
        glScissor((vp.size.x - pointSize) / 2, (vp.size.y - pointSize) / 2, pointSize, pointSize);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT); // draw point
    }
    glDisable(GL_SCISSOR_TEST);
}

int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;
    Viewport window({800, 800});

    // game seed
    std::hash<std::string> hashString;
    std::srand(hashString("Moncraft"));

    // load resources
    loadResources();
    Shader* shader = ResourceManager::getShader("simple");
    GLuint texAtlas = ResourceManager::getTexture("atlas");
    GLuint texCharacter = ResourceManager::getTexture("character");
    GLuint normalMapID[30];
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }

    // init objects
    Terrain terrain;
    SkyBox sky;
    Raycast caster(100.f);
    ShadowMap shadows(4096);
    Character character({ 0.0f, 40.0f, 0.0f });
    Music MusicPlayer;
    float t = 0;


    // UI stuff
    auto font_roboto = std::make_shared<const Font>("Roboto-Regular");
    auto font_vt323 = std::make_shared<const Font>("VT323-Regular");

    auto scene = std::make_shared<ui::Root>(window.size);
    window.setScene(scene);

    ui::Pane pane_fps(scene.get());
    // pane_fps.setColor({ 1.f, 1.f, 1.f, 0.5f });
    // pane_fps.setPosition({ -10, -10 });
    // pane_fps.setAnchorY(ui::Anchor::END);
    // pane_fps.setAnchorX(ui::Anchor::END);
    // pane_fps.setPadding({ 10, 10 });
    // pane_fps.setSize({ 300, 10 });
    // pane_fps.setStyle(ui::make_property(ui::Pane::COLOR, vec4{1.f, 1.f, 1.f, 0.5f}));
    pane_fps.setStyle(ui::make_property(ui::Pane::POSITION, ivec2{-10, -10}));
    pane_fps.setStyle(ui::make_property(ui::Pane::ANCHOR_X, ui::Anchor::END));
    pane_fps.setStyle(ui::make_property(ui::Pane::ANCHOR_Y, ui::Anchor::END));
    pane_fps.setStyle(ui::make_property(ui::Pane::PADDING, ivec2{10, 10}));
    pane_fps.setStyle(ui::make_property(ui::Pane::SIZE, ivec2{300, 10}));

    ui::Text text_fps(&pane_fps, "hello", font_vt323);
    // text_fps.setFontSize(2.f);
    // text_fps.setColor({ 0.8f, 0.7f, 0.0f, 1.f });
    text_fps.setStyle(ui::make_property(ui::Text::FONT_SIZE, 2.f));
    text_fps.setStyle(ui::make_property<vec4>(ui::Text::COLOR, { 0.8f, 0.7f, 0.0f, 1.f }));

    ui::Button btn(scene.get(), "hello", font_vt323);

    // main loop
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {
        t += dt;

        // updates
        MusicPlayer.update();
        window.keyboardController.apply(character, terrain);
        window.mouseController.apply(character, terrain);
        character.update(terrain, dt);
        character.cameraToHead(window.camera);
        terrain.update(window.camera.position);


        // update sun
        float sunSpeed = 5.f;
        float sunTime = pi<float>() * .25f;
        float sunDist = 100.f;
        auto sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * sunDist;
        auto sunDirViewSpace = window.camera.view * vec4(sunDir, 0.0);
        sunTime += t / 300.f * sunSpeed;


        // draw the shadow map
        shadows.update(sunDir);
        shadows.attach(window.camera, Frustum::NEAR);
        shadows.beginFrame(Frustum::NEAR);
        terrain.render(shadows.camera);
        character.render();

        shadows.attach(window.camera, Frustum::MEDIUM);
        shadows.beginFrame(Frustum::MEDIUM);
        terrain.render(shadows.camera);

        shadows.attach(window.camera, Frustum::FAR);
        shadows.beginFrame(Frustum::FAR);
        terrain.render(shadows.camera);
        shadows.endFrame();


        // prepare render
        shader->activate();
        window.camera.activate();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // set uniforms / textures
        glUniform1f(shader->getUniformLocation("lightIntensity"), 1);
        glUniform3fv(shader->getUniformLocation("lightDirection"), 1, value_ptr(sunDirViewSpace));
        glUniform1f(shader->getUniformLocation("sunTime"), sunTime * 400);
        glUniform1i(shader->getUniformLocation("fog"), (int)window.enableFog);
        shader->bindTexture(TEXTURE_NORMAL, normalMapID[(size_t)(t*15)%30]);

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


        // draw skybox
        sky.render(window.camera);
        shader->activate();
        shadows.activate();
        window.camera.activate();


        // draw the terrain
        shader->bindTexture(TEXTURE_COLOR, texAtlas);
        terrain.render(window.camera);


        // draw dot in the middle of the screen
        drawMiddleDot(window);


        // draw character
        if (character.view == View::THIRD_PERSON) {
            shader->bindTexture(TEXTURE_COLOR, texCharacter);
            character.render();
        }


        // draw ui
        std::string text = "FPS : " + std::to_string((int)(1.f / dt));
        text_fps.setText(text);
        scene->update();
        scene->render();
    }

    ResourceManager::free();
    return 0;
}
