#include <glm/gtc/type_ptr.hpp>

#include "MonCraftScene.hpp"
#include "gl/ResourceManager.hpp"
#include "multiplayer/common/Config.hpp"

using namespace glm;

MonCraftScene::MonCraftScene(Viewport* vp, entities_ptr_t entities)
    : ui::Component(vp->getRoot()), vp(vp),
      camera(ivec2(1), {0, 32, 10}, {0, 32, 0}),

      caster(100.f),
      shadows(4096),
      entities(entities),

      fogEnabled(false),
      sunSpeed(0.0075f), skyboxSpeed(0.0075f),
      captured(false)//, save("save/defaultWorld/entities")
{
    // load resources
    shader = ResourceManager::getShader("simple");
    fogShader = ResourceManager::getShader("fog");
    texAtlas = ResourceManager::getTexture("atlas");
    texCharacter = ResourceManager::getTexture("character");
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }

    entities->initialize();
}

bool MonCraftScene::onMousePressed(glm::ivec2 pos) {
    vp->captureMouse();
    captured = true;
    return true;
}
bool MonCraftScene::onMouseMove(glm::ivec2 pos) {
    if(captured) return true;
    return false;
}

void MonCraftScene::updateShadowMaps() {
    shadows.update(sunDir);
    shadows.attach(camera, Frustum::NEAR);
    shadows.beginFrame(Frustum::NEAR);
    entities->terrain->render(shadows.camera);
    entities->player->render();

    shadows.attach(camera, Frustum::MEDIUM);
    shadows.beginFrame(Frustum::MEDIUM);
    entities->terrain->render(shadows.camera);

    shadows.attach(camera, Frustum::FAR);
    shadows.beginFrame(Frustum::FAR);
    entities->terrain->render(shadows.camera);
    shadows.endFrame();
}


void MonCraftScene::updateUniforms(float t) {
    auto sunDirViewSpace = camera.view * vec4(sunDir, 0.0);

    glUniform1f(shader->getUniformLocation("lightIntensity"), 1);
    glUniform1f(shader->getUniformLocation("skyTime"), t * skyboxSpeed);
    glUniform3fv(shader->getUniformLocation("lightDirection"), 1, value_ptr(sunDirViewSpace));
    glUniform1f(fogShader->getUniformLocation("sunTime"), t);
    glUniform1f(fogShader->getUniformLocation("lightIntensity"), 1);
    glUniform3fv(fogShader->getUniformLocation("lightDirection"), 1, value_ptr(sunDirViewSpace));
    glUniform1i(shader->getUniformLocation("fog"), (int)fogEnabled); // TODO
    shader->bindTexture(TEXTURE_NORMAL, normalMapID[(size_t)(t*15)%30]);

    Block* block = entities->terrain->getBlock(ivec3(camera.position + vec3(-0.5f,0.6f,-0.5f)));
    if (block) {
        bool isUnderWater = block->type == BlockType::Water;
        GLint underWater = shader->getUniformLocation("underWater");
        glUniform1i(underWater, isUnderWater);

        sky.skyBoxShader->activate();
        underWater = sky.skyBoxShader->getUniformLocation("underWater");
        glUniform1i(underWater, isUnderWater);

        shader->activate();
    }
}

void MonCraftScene::drawMiddleDot() {
    glEnable(GL_SCISSOR_TEST);
    {
        auto size = getSize();
        float pointSize = 8;
        // TODO: make this properly use compnent position and size
        glScissor((size.x - pointSize) / 2, (size.y - pointSize) / 2, pointSize, pointSize);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT); // draw point
    }
    glDisable(GL_SCISSOR_TEST);
}

void MonCraftScene::drawSkybox(float t) {
    sky.render(camera, t * skyboxSpeed);
    shader->activate();
    shadows.activate();
    camera.activate();
}

void MonCraftScene::drawTerrain() {
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    entities->terrain->render(camera);
}

void MonCraftScene::drawCharacter() {
    if (entities->player->view == View::THIRD_PERSON) {
        shader->bindTexture(TEXTURE_COLOR, texCharacter);
        entities->player->render();
    }
    for(auto& pair : entities->players) {
        shader->bindTexture(TEXTURE_COLOR, texCharacter);
        pair.second->render();
    }
}

void MonCraftScene::drawFrame(float t, float dt) {
    // updates
    musicPlayer.update();
    vp->keyboardController.apply(*entities->player, *entities->terrain);
    vp->mouseController.apply(*entities->player, *entities->terrain);
    entities->player->update(*entities->terrain, dt);
    for(auto& pair : entities->players) pair.second->update(*entities->terrain, dt);
    setSize(parent->getSize());
    camera.setSize(getSize());
    entities->player->cameraToHead(camera);
    entities->terrain->update(camera.position);

    // update sun
    float sunTime = quarter_pi<float>() + t * sunSpeed; // sun is fixed
    float sunDist = 100.f;
    sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * sunDist;

    // update the shadow map
    updateShadowMaps();

    // prepare render
    shader->activate();
    camera.activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set uniforms / textures
    updateUniforms(t);

    // draw skybox
    drawSkybox(t);

    // draw the terrain
    drawTerrain();

    // draw dot in the middle of the screen
    drawMiddleDot();

    // draw character
    drawCharacter();
}
