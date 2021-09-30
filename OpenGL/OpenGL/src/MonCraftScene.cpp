#include <glm/gtc/type_ptr.hpp>

#include "MonCraftScene.hpp"
#include "gl/ResourceManager.hpp"
#include "multiplayer/common/Config.hpp"
#include "multiplayer/client/Server.hpp"
#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "ui/ui.hpp"

using namespace glm;

MonCraftScene::MonCraftScene(Viewport* vp)
    : ui::Component(vp->getRoot()),
      world(World::getInst()),
      vp(vp),
      camera(ivec2(1), {0, 32, 10}, {0, 32, 0}),

      caster(100.f),
      shadows(4096),

      fogEnabled(false),
      sunSpeed(0.0075f), skyboxSpeed(0.0075f),
      captured(false)
{
    // load resources
    shader = ResourceManager::getShader("simple");
    fogShader = ResourceManager::getShader("fog");
    texAtlas = ResourceManager::getTexture("atlas");
    texCharacter = ResourceManager::getTexture("character");
    for (size_t i = 0; i < 30; i += 1) {
        normalMapID[i] = ResourceManager::getTexture("waterNormal" + std::to_string(i));
    }
}

std::unique_ptr<Server> createServer(Config const& config) {
    std::unique_ptr<Server> server;
    if (config.multiplayer) {
        server = std::make_unique<RealServer>(config.serverAddr, config.serverPort);
    }
    else {
        server = std::make_unique<ClientServer>();
    }
    return server;
}

void MonCraftScene::drawMoncraftWorld() {
    Config config = SaveManager::getInst().getConfig();

    // game seed
    std::hash<std::string> hashString;
    auto seed = hashString(config.seed);
    std::srand(seed);
    std::cout << "seed : " << config.seed << " (" << seed << ")" << std::endl;



    auto server = createServer(config);
    World& world = World::getInst();

    player = server->getPlayer();
    float t = 0;

    // UI stuff
    auto font_roboto = std::make_shared<const Font>("Roboto-Regular");
    auto font_vt323 = std::make_shared<const Font>("VT323-Regular");

    setPadding({ 10, 10 });

    ui::Pane pane_fps(this);
    pane_fps.setColor({ 1.f, 1.f, 1.f, 0.5f });
    pane_fps.setPosition({ -10, -10 });
    pane_fps.setAnchorY(ui::Anchor::END);
    pane_fps.setAnchorX(ui::Anchor::END);
    pane_fps.setPadding({ 10, 10 });
    pane_fps.setSize({ 300, 10 });

    ui::Text text_fps(&pane_fps, "", font_vt323);
    text_fps.setFontSize(2.f);
    text_fps.setColor({ 0.8f, 0.7f, 0.0f, 1.f });

    ui::Button btn_vsync(this, "VSync", font_vt323);
    btn_vsync.setPadding({ 15, 10 });

    ui::Button btn_fullscreen(this, "Fullscreen", font_vt323);
    btn_fullscreen.setAnchorX(ui::Anchor::END);
    btn_fullscreen.setPadding({ 15, 10 });

    ui::Button btn_ping(this, "Ping", font_vt323);
    btn_ping.setPosition({ 0, 80 }); // TODO: implement a box container
    btn_fullscreen.setPadding({ 15, 10 });

    btn_vsync.onclick([&] { vp->toggleVSync(); });
    btn_fullscreen.onclick([&] { vp->toggleFullscreen(); });
    btn_ping.onclick([&] { server->ping(); });

    ui::Text text_posPlayer(this, "", font_vt323);
    text_posPlayer.setAnchorY(ui::Anchor::END);
    text_posPlayer.setFontSize(.5f);

    ui::Text text_gameTime(this, "", font_vt323);
    text_gameTime.setAnchorY(ui::Anchor::END);
    text_gameTime.setPosition(ivec2(0, -90)); // TODO: implement a box container
    text_gameTime.setFontSize(.5f);

    ui::Text text_players(this, "", font_vt323);
    text_players.setAnchorY(ui::Anchor::END);
    text_players.setPosition({ 0, -30 }); // TODO: implement a box container
    text_players.setFontSize(.5f);

    ui::Text text_uid(this, "", font_vt323);
    text_uid.setAnchorY(ui::Anchor::END);
    text_uid.setPosition({ 0, -60 }); // TODO: implement a box container
    text_uid.setFontSize(.5f);

    // main loop
    for (float dt = 0; vp->beginFrame(dt); vp->endFrame()) {
        t += dt;

        server->update();

        drawFrame(t, dt);

        std::ostringstream text;
        text << "FPS : " << (int)(1.f / dt);
        text_fps.setText(text.str());

        text.str(""); // "clears" the string stream
        text << "Player Pos : " << std::fixed << std::setprecision(3) << server->getPlayer()->getPosition();
        text_posPlayer.setText(text.str());

        text.str(""); // "clears" the string stream
        text << "Players online : " << world.entities.count();
        text_players.setText(text.str());

        text.str(""); // "clears" the string stream
        text << "UID : " << server->getPlayer()->uid;
        text_uid.setText(text.str());

        text.str(""); // "clears" the string stream
        text << "Game Time : " << std::fixed << std::setprecision(3) << t;
        text_gameTime.setText(text.str());
    }

    ResourceManager::free();
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
    #ifndef EMSCRIPTEN // TODO: COMBAK: we need to find a way to make it work in wasm
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
    #endif
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
    shader->bindTexture(TEXTURE_NORMAL, normalMapID[(size_t)(t*15)%30]);

    Block* block = world.getBlock(ivec3(camera.position + vec3(-0.5f,0.6f,-0.5f)));
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

void MonCraftScene::drawFrame(float t, float dt) {
    // updates
    #ifndef EMSCRIPTEN
        musicPlayer.update();
    #endif

    vp->keyboardController.apply(*player);
    vp->mouseController.apply(*player);

    world.entities.updateAll(dt);

    setSize(parent->getSize());
    camera.setSize(getSize());

    player->cameraToHead(camera);

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
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    world.render(camera);

    // draw the entities
    drawEntities();

    // draw dot in the middle of the screen
    drawMiddleDot();
}
