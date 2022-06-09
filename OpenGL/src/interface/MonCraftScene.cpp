#include "MonCraftScene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stddef.h>
#include <cmath>
#include <string>
#include <unordered_map>
#include <utility>

#include "blocks/AllBlocks.hpp"
#include "blocks/Block.hpp"
#include "controller/MouseController.hpp"
#include "entity/Entities.hpp"
#include "entity/Entity.hpp"
#include "entity/character/Character.hpp"
#include "gl/ResourceManager.hpp"
#include "gl/Shader.hpp"
#include "gl/Viewport.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/client/Server.hpp"
#include "noise/prng.hpp"
#include "save/ClientConfig.hpp"
#include "save/ServerConfig.hpp"
#include "terrain/World.hpp"
#include "ui/Button.hpp"
#include "ui/style/Type.hpp"

using namespace glm;
using namespace ui;

std::shared_ptr<Server> createServer(bool multiplayer) {
    auto& cconf = Config::getClientConfig();
    auto& sconf = Config::getServerConfig();

    // game seed
    auto seed = prng::srands(sconf.seed);
    std::cout << "seed : " << sconf.seed << " (" << seed << ")" << std::endl;

    std::unique_ptr<Server> server;
    if (multiplayer) {
        server = std::make_unique<RealServer>(cconf.serverAddr, cconf.serverPort);
    }
    else {
        server = std::make_unique<ClientServer>();
    }
    return server;
}


MonCraftScene::MonCraftScene(Viewport* vp)
    : Component(),
      world(World::getInst()),
      config(Config::getClientConfig()),
      vp(vp),
      camera(ivec2(1)),
      shadows(4096),
      sunSpeed(0.0075f)
{
    auto const& serverConf = Config::getServerConfig();
    camera.setFar(16.0f * (float)sqrt(2 * pow(serverConf.renderDistH, 2) + pow(serverConf.renderDistV, 2)));
    camera.setFovY(config.fov);

    // load resources
    shader = ResourceManager::getShader("simple");
    texAtlas = ResourceManager::getTexture("atlas");
    texCharacter = ResourceManager::getTexture("character");
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }

    server = createServer(Config::getClientConfig().multiplayer);
    server->start();
    player = server->getPlayer();
    playerUid = server->getUid();

    // UI stuff
    gameMenu = GameMenu::create();
    debugOverlay = std::make_unique<DebugOverlay>(server);
    overlay = std::make_unique<Overlay>();
    middleDot = Image::create({1, 1229}, {10, 10});

    debugOverlay->setAnchorY(Anchor::END);

    middleDot->setAnchorX(Anchor::CENTER);
    middleDot->setAnchorY(Anchor::CENTER);

    middleDot->setSize({10, 10});

    gameMenu->setAnchorX(Anchor::CENTER);
    gameMenu->setAnchorY(Anchor::CENTER);

    add(middleDot.get());
    add(overlay.get());
    add(debugOverlay.get());

    player->setCurrentBlock(overlay->getCurrentBlock());
}

bool MonCraftScene::onMousePressed(glm::ivec2 pos) {
    makeActive();
    return true;
}

bool MonCraftScene::onMouseMove(glm::ivec2 pos) {
    return vp->isMouseCaptured();
}

void MonCraftScene::onKeyPressed(Key k) {
    if(vp->isMouseCaptured())
        keyboardController.handleKeyPressed(k);
}

void MonCraftScene::onKeyReleased(Key k) {
    if(vp->isMouseCaptured())
        keyboardController.handleKeyReleased(k);
    if(k.asKeycode() == Config::getClientConfig().menu) {
        if(vp->isMouseCaptured()) {
            add(gameMenu.get());
            vp->freeMouse();
        }
        else {
            remove(gameMenu.get());
            vp->captureMouse();
        }
    }
}

void MonCraftScene::updateShadowMaps() {
    shadows.update(sunDir);
    shadows.attach(camera, Frustum::NEAR);
    shadows.beginFrame(Frustum::NEAR);
    renderer.renderSolid(shadows.camera);
    world.entities.renderAll();

    shadows.attach(camera, Frustum::MEDIUM);
    shadows.beginFrame(Frustum::MEDIUM);
    renderer.renderSolid(shadows.camera);

    shadows.attach(camera, Frustum::FAR);
    shadows.beginFrame(Frustum::FAR);
    renderer.renderSolid(shadows.camera);
    shadows.endFrame();
}

void MonCraftScene::updateUniforms(uint32_t t) {
    auto sunDirViewSpace = camera.view * vec4(sunDir, 0.0);

    glUniform1f(shader->getUniform("lightIntensity"), 1);
    glUniform1f(shader->getUniform("sunAmount"), 1.0f - sky.getBlendFactor());
    glUniform3fv(shader->getUniform("lightDirection"), 1, value_ptr(sunDirViewSpace));
    size_t normalMapIndex = (size_t)(t * 0.001f * 15) % 30; //TODO check day change
    shader->bindTexture(TEXTURE_NORMAL, normalMapID[normalMapIndex]);

    Block* block = world.getBlock(ivec3(camera.position + vec3(-0.5f, 0.6f, -0.5f)));
    if (block) {
        bool isUnderWater = block->type == BlockType::Water;
        GLint flags;
        glGetUniformiv(shader->program, shader->getUniform("flags"), &flags);
        flags = (flags & 0b1110) | ((GLint)isUnderWater & 0b0001);
        glUniform1i(shader->getUniform("flags"), flags);

        sky.skyBoxShader->activate();
        glUniform1i(sky.skyBoxShader->getUniform("flags"), flags);

        shader->activate();
    }
}

void MonCraftScene::updateFov(uint32_t dt) {
  const float maxFov = 180.0f;
  const float smoothing = 0.005f;
  const float transition = 10.f;
  const float speed = glm::length(player->speed);
  const float fov = camera.getFovY();
  const auto targetFov = fov - (maxFov + (config.fov - maxFov) * exp(-smoothing * speed));
  camera.setFovY(fov - targetFov * transition * dt * 0.001f);
}

void MonCraftScene::drawSkybox() {
    sky.render(camera);
    shader->activate();
    shadows.activate();
    camera.activate();
}

void MonCraftScene::drawEntities() {
    shader->bindTexture(TEXTURE_COLOR, texCharacter);
    for(auto pair : world.entities) {
        if(pair.first == playerUid) {
            if(player->view != CharacterView::FIRST_PERSON) 
            {
                player->render();
                middleDot->setHidden(true);
            }
            else
                middleDot->setHidden(false);
        }
        else {
            pair.second->render();
        }
    }
}

void MonCraftScene::draw() {
    glEnable(GL_DEPTH_TEST);

    // updates
    #ifndef EMSCRIPTEN
      musicPlayer.update();
    #endif

    if(overlay->select((int)(player->getCurrentBlock()) + vp->getMouseScrollDiff()))
        player->setCurrentBlock(overlay->getCurrentBlock());

    keyboardController.apply(*player);
    vp->mouseController.apply(*player);

    world.entities.updateAll(world.dt);

    setSize(parent->getSize());
    camera.setSize(getSize());

    player->cameraToHead(camera);
    updateFov(world.dt);

    // update sun
    float sunTime = quarter_pi<float>() + world.t * sunSpeed * 0.001f; // sun is fixed
    float sunDist = 100.f;
    sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * sunDist;

    // update the shadow map
    #ifndef EMSCRIPTEN
      updateShadowMaps();
    #endif

    // prepare render
    shader->activate();
    camera.activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set uniforms / textures
    updateUniforms(world.t);

    // draw skybox
    drawSkybox();

    // draw the terrain
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    renderer.render(camera);

    // draw the entities
    drawEntities();

    glDisable(GL_DEPTH_TEST);
    Component::draw();
}
