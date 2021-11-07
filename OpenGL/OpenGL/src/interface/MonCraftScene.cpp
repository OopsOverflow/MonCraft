#include <glm/gtc/type_ptr.hpp>

#include "MonCraftScene.hpp"
#include "gl/ResourceManager.hpp"
#include "save/ClientConfig.hpp"
#include "save/ServerConfig.hpp"

#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "ui/ui.hpp"
#include "debug/Debug.hpp"

using namespace glm;
using namespace ui;

std::shared_ptr<Server> createServer() {
    auto& config = Config::getClientConfig();
    std::unique_ptr<Server> server;
    if (config.multiplayer) {
        server = std::make_unique<RealServer>(config.serverAddr, config.serverPort);
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
      caster(100.f),
      shadows(4096),
      fogEnabled(false),
      sunSpeed(0.0075f), skyboxSpeed(0.0075f),
      captured(false)
{
    auto const& serverConf = Config::getServerConfig();
    camera.setFar(16.0f * (float)sqrt(2 * pow(serverConf.renderDistH, 2) + pow(serverConf.renderDistV, 2)));
    camera.setFovY(config.fov);

    // load resources
    shader = ResourceManager::getShader("simple");
    fogShader = ResourceManager::getShader("fog");
    texAtlas = ResourceManager::getTexture("atlas");
    texCharacter = ResourceManager::getTexture("character");
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }

    server = createServer();
    server->start();
    player = server->getPlayer();
    playerUid = server->getUid();

    // UI stuff
    gameMenu = std::make_unique<GameMenu>();
    debugOverlay = std::make_unique<DebugOverlay>(server);
    overlay = std::make_unique<Overlay>();
    middleDot = Image::create(glm::ivec2(1, 1230), glm::ivec2(10, 10));

    debugOverlay->setAnchorY(Anchor::END);

    middleDot->setAnchorX(Anchor::CENTER);
    middleDot->setAnchorY(Anchor::CENTER);

    middleDot->setSize(glm::ivec2(10, 10));

    overlay->btn_block->onclick([&] {
        auto prev = player->getCurrentBlock();
        player->setCurrentBlock(AllBlocks::nextBlock(prev));
    });

    add(middleDot.get());
    add(overlay.get());
    add(debugOverlay.get());
}

bool MonCraftScene::onMousePressed(glm::ivec2 pos) {
    vp->captureMouse();
    captured = true;
    makeActive();
    return true;
}

bool MonCraftScene::onMouseMove(glm::ivec2 pos) {
    if(captured) return true;
    return false;
}

bool MonCraftScene::onActivate() {
    return true;
}

void MonCraftScene::onKeyPressed(Key k) {
    keyboardController.handleKeyPressed(k);
}

void MonCraftScene::onKeyReleased(Key k) {
    keyboardController.handleKeyReleased(k);
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

void MonCraftScene::updateUniforms(float t) {
    auto sunDirViewSpace = camera.view * vec4(sunDir, 0.0);

    glUniform1f(shader->getUniform("lightIntensity"), 1);
    glUniform1f(shader->getUniform("skyTime"), t * skyboxSpeed);
    glUniform3fv(shader->getUniform("lightDirection"), 1, value_ptr(sunDirViewSpace));
    glUniform1f(fogShader->getUniform("sunTime"), t);
    glUniform1f(fogShader->getUniform("lightIntensity"), 1);
    glUniform3fv(fogShader->getUniform("lightDirection"), 1, value_ptr(sunDirViewSpace));
    glUniform1i(shader->getUniform("fog"), (int)fogEnabled); // TODO
    size_t normalMapIndex = (size_t)(t * 15) % 30;
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

void MonCraftScene::updateFov(float dt) {
  const float maxFov = 180.0f;
  const float smoothing = 0.005f;
  const float transition = 10.f;
  const float speed = glm::length(player->speed);
  const float fov = camera.getFovY();
  const auto targetFov = fov - (maxFov + (config.fov - maxFov) * exp(-smoothing * speed));
  camera.setFovY(fov - targetFov * transition * dt);
}

void MonCraftScene::drawSkybox(float t) {
    sky.render(camera, t * skyboxSpeed);
    shader->activate();
    shadows.activate();
    camera.activate();
}

void MonCraftScene::drawEntities() {
    shader->bindTexture(TEXTURE_COLOR, texCharacter);
    for(auto pair : world.entities) {
        if(pair.first == playerUid) {
            if(player->view == CharacterView::THIRD_PERSON)
                player->render();
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

    keyboardController.apply(*player);
    vp->mouseController.apply(*player);

    world.entities.updateAll(world.dt);

    setSize(parent->getSize());
    camera.setSize(getSize());

    player->cameraToHead(camera);
    updateFov(world.dt);

    // update sun
    float sunTime = quarter_pi<float>() + world.t * sunSpeed; // sun is fixed
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
    drawSkybox(world.t);

    // draw the terrain
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    renderer.render(camera);

    // draw the entities
    drawEntities();

    glDisable(GL_DEPTH_TEST);
    Component::draw();
}
