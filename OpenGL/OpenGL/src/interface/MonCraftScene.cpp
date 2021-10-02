#include <glm/gtc/type_ptr.hpp>

#include "MonCraftScene.hpp"
#include "gl/ResourceManager.hpp"
#include "multiplayer/common/Config.hpp"

#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "ui/ui.hpp"
#include "util/SaveManager.hpp"
#include "debug/Debug.hpp"

using namespace glm;

std::shared_ptr<Server> createServer(Config const& config) {
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
    : ui::Component(),
      world(World::getInst()),
      vp(vp),
      camera(ivec2(1), {0, 32, 10}, {0, 32, 0}),
      caster(100.f),
      shadows(4096),
      fogEnabled(false),
      sunSpeed(0.0075f), skyboxSpeed(0.0075f),
      captured(false)
{
    Config config = SaveManager::getInst().getConfig();

    // load resources
    shader = ResourceManager::getShader("simple");
    fogShader = ResourceManager::getShader("fog");
    texAtlas = ResourceManager::getTexture("atlas");
    texCharacter = ResourceManager::getTexture("character");
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }

    // game seed
    std::hash<std::string> hashString;
    auto seed = hashString(config.seed);
    std::srand(seed);
    std::cout << "seed : " << config.seed << " (" << seed << ")" << std::endl;

    World& world = World::getInst();
    server = createServer(config);
    player = server->getPlayer();

    // UI stuff
    gameMenu = std::make_unique<GameMenu>();
    debugOverlay = std::make_unique<DebugOverlay>(server);
    overlay = std::make_unique<Overlay>();

    debugOverlay->setAnchorY(ui::Anchor::END);

    overlay->btn_vsync->onclick([=] { vp->toggleVSync(); });
    overlay->btn_fullscreen->onclick([=] { vp->toggleFullscreen(); });
    overlay->btn_ping->onclick([&] { server->ping(); });
    addChild(overlay.get());
    addChild(debugOverlay.get());

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
    world.render(shadows.camera);
    world.entities.renderAll();

    shadows.attach(camera, Frustum::MEDIUM);
    shadows.beginFrame(Frustum::MEDIUM);
    world.render(shadows.camera);

    shadows.attach(camera, Frustum::FAR);
    shadows.beginFrame(Frustum::FAR);
    world.render(shadows.camera);
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
    shader->bindTexture(TEXTURE_NORMAL, normalMapID[(size_t)(t * 15) % 30]);

    Block* block = world.getBlock(ivec3(camera.position + vec3(-0.5f, 0.6f, -0.5f)));
    if (block) {
        bool isUnderWater = block->type == BlockType::Water;
        GLint underWater = shader->getUniform("underWater");
        glUniform1i(underWater, isUnderWater);

        sky.skyBoxShader->activate();
        underWater = sky.skyBoxShader->getUniform("underWater");
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

void MonCraftScene::drawEntities() {
    shader->bindTexture(TEXTURE_COLOR, texCharacter);
    for(auto pair : world.entities) {
        if(pair.first == player->uid) {
            if(player->view == View::THIRD_PERSON)
                player->render();
        }
        else {
            pair.second->render();
        }
    }
}

void MonCraftScene::draw() {

    glEnable(GL_DEPTH_TEST);
    server->update();

    // updates
    musicPlayer.update();
    vp->keyboardController.apply(*player);
    vp->mouseController.apply(*player);

    world.entities.updateAll(world.dt);

    setSize(parent->getSize());
    camera.setSize(getSize());

    player->cameraToHead(camera);

    // update sun
    float sunTime = quarter_pi<float>() + world.t * sunSpeed; // sun is fixed
    float sunDist = 100.f;
    sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * sunDist;

    // update the shadow map
    updateShadowMaps();

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
    world.render(camera);

    // draw the entities
    drawEntities();

    glDisable(GL_DEPTH_TEST);
    // draw dot in the middle of the screen
    drawMiddleDot();

    Component::draw();
}
