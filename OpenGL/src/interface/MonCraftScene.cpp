#include "MonCraftScene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
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
    spdlog::info("Seed: {} ({})", sconf.seed, seed);

    World::getInst().unload(); // ensure the world is reset before creating the server

    std::unique_ptr<Server> server;
    if (multiplayer) {
        server = std::make_unique<RealServer>(cconf.serverAddr, cconf.serverPort, cconf.serverTLS);
    }
    else {
        server = std::make_unique<ClientServer>();
    }
    return server;
}

void cameraToHead(Camera& camera, Entity& entity) {
  vec3 eyePos;
  vec3 eyeTarget;
	if(entity.view == PlayerView::FIRST_PERSON) {
		eyePos = entity.headNode.model * vec4(0, 4, 0, 1);
		eyeTarget = entity.headNode.model * vec4(0, 4, 50, 1);

	}
	else{
    if(entity.view == PlayerView::THIRD_PERSON){
	    eyePos = entity.headNode.model * vec4(0, 4, 1, 1);
      eyeTarget = entity.headNode.model * vec4(0, 4, 0, 1);

    }
    else if(entity.view == PlayerView::FRONT) {
      eyePos = entity.headNode.model * vec4(0, 4, -1, 1);
      eyeTarget = entity.headNode.model * vec4(0, 4, 0, 1);
    }
    
    camera.setPosition(eyeTarget);
    camera.setDirection(eyePos - eyeTarget);
    auto tmp = camera.getBoxCorners();
    
    float min = 4.f;
    for(size_t i = 0; i < 4; i += 1) {
      auto cast = entity.caster.blockCast(tmp.at(i), eyeTarget - eyePos, min);

      min = std::min(min, cast.dist);
    }
    if(min > 0.5f) 
      eyePos = eyeTarget + min * normalize(eyeTarget - eyePos);
    else
      eyePos = eyeTarget + 0.5f * normalize(eyeTarget - eyePos);
    } 

    camera.setPosition(eyePos);
    camera.setDirection(eyeTarget - eyePos);
}

MonCraftScene::MonCraftScene(Viewport* vp)
    : Component(),
      world(World::getInst()),
      config(Config::getClientConfig()),
      vp(vp),
      camera(ivec2(1)),
      #ifndef EMSCRIPTEN
          shadows(4096),
      #endif
      sunSpeed(0.0075f),
      lastClock(SDL_GetTicks()),
      server(createServer(Config::getClientConfig().multiplayer)),
      playerController(nullptr), debug(false)
{
    playerController = std::make_unique<PlayerController>(server->getPlayer());
    
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

    playerUid = server->getUid();

    // UI stuff
    gameMenu = GameMenu::create();
    debugOverlay = DebugOverlay::create(server);
    overlay = Overlay::create();
    middleDot = Image::create({1, 1229}, {10, 10});

    parameters = ParametersMenu::create();
    parameters->quitButton->onClick([params = parameters.get(), this] { 
        this->remove(params); 
        this->makeActive();
    });
    parameters->graphicsMenu->fullscreen->onRelease([&, vp, fullscreen = parameters->graphicsMenu->fullscreen.get()] { 
        config.fullscreen = fullscreen->getChecked();
        vp->toggleFullscreen();
    });
    parameters->graphicsMenu->vsync->onRelease([&, vp, vsync = parameters->graphicsMenu->vsync.get()] { 
        config.vsync = vsync->getChecked();
        vp->toggleVSync();
    });

    gameMenu->parameterButton->onClick([param = parameters, this] {
        this->add(param);
        this->makeActive();
    });
    
    #ifndef EMSCRIPTEN
    parameters->audioMenu->mainVolume->onRelease([=]{ 
        config.mainVolume = (float)parameters->audioMenu->mainVolume->getValue(); 
        musicPlayer.music.setVolume(config.mainVolume * config.musicVolume * 0.01f);
    });
	parameters->audioMenu->musicVolume->onRelease([=]{
        config.musicVolume = (float)parameters->audioMenu->musicVolume->getValue();
        musicPlayer.music.setVolume(config.mainVolume * config.musicVolume * 0.01f);
    });
    #endif

    debugOverlay->setAnchorY(Anchor::END);

    middleDot->setAnchorX(Anchor::CENTER);
    middleDot->setAnchorY(Anchor::CENTER);

    middleDot->setSize({10, 10});

    gameMenu->setAnchorX(Anchor::CENTER);
    gameMenu->setAnchorY(Anchor::CENTER);

    add(middleDot);
    add(overlay);

    auto player = std::dynamic_pointer_cast<Character>(server->getPlayer());
    if(player)
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
        playerController->handleKeyPressed(k);
}

void MonCraftScene::onKeyReleased(Key k) {
    if(vp->isMouseCaptured())
        playerController->handleKeyReleased(k);
    if(k == Config::getClientConfig().menu) {
        bool paramDisplayed = children.end() != std::find_if(children.begin(), children.end(), [&](auto& other) {
            return other.get() == parameters.get();
        });
        if(paramDisplayed) {
            this->remove(parameters.get()); 
        }else{
            if(vp->isMouseCaptured() ) {
                add(gameMenu);
                vp->freeMouse();
            }
            else {
                remove(gameMenu.get());
                vp->captureMouse();
            }
        }
    }
    if(k == SDLK_F3) {
        debug = !debug;
        if(debug)
            add(debugOverlay);
        else
            remove(debugOverlay.get());
    }
        
}

#ifndef EMSCRIPTEN
void MonCraftScene::updateShadowMaps() {
    shadows.update(sunDir);
    shadows.attach(camera, Frustum::NEAR);
    shadows.beginFrame(Frustum::NEAR);
    renderer.renderSolid(renderer.visibleChunks(shadows.camera));
    world.entities.renderAll();

    shadows.attach(camera, Frustum::MEDIUM);
    shadows.beginFrame(Frustum::MEDIUM);
    renderer.renderSolid(renderer.visibleChunks(shadows.camera));

    shadows.attach(camera, Frustum::FAR);
    shadows.beginFrame(Frustum::FAR);
    renderer.renderSolid(renderer.visibleChunks(shadows.camera));
    shadows.endFrame();
}
#endif

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
  const float speed = glm::length(playerController->getEntity()->speed);
  const float fov = camera.getFovY();
  const auto targetFov = fov - (maxFov + (config.fov - maxFov) * exp(-smoothing * speed));
  camera.setFovY(fov - targetFov * transition * dt * 0.001f);
}

void MonCraftScene::drawSkybox() {
    sky.render(camera);
    shader->activate();
    camera.activate();
}

void MonCraftScene::drawEntities() {
    shader->bindTexture(TEXTURE_COLOR, texCharacter);
    for(auto pair : world.entities) {
        if(pair.first == playerUid) {
            if(playerController->getEntity()->view != PlayerView::FIRST_PERSON) 
            {
                playerController->getEntity()->render();
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
    uint32_t time = SDL_GetTicks();
    world.dt = time - lastClock; 
    world.t += world.dt;
    world.t = world.t % dayDuration;
    lastClock = time;

    server->update();
    if(server->getState() == ServerState::DISCONNECTED) {
        gameMenu->quitButton->click();
        vp->freeMouse();
        return;
    }
    

    // updates
    #ifndef EMSCRIPTEN
      musicPlayer.update();
    #endif

    auto player = std::dynamic_pointer_cast<Character>(server->getPlayer());
    if(player && overlay->select((int)(player->getCurrentBlock()) + vp->getMouseScrollDiff()))
        player->setCurrentBlock(overlay->getCurrentBlock());

    // keyboardController.apply(*player);
    vp->mouseController.apply(*playerController);
    playerController->update();

    world.entities.updateAll(world.dt);

    setSize(parent->getSize());
    camera.setSize(getSize());

    cameraToHead(camera, *playerController->getEntity());
    updateFov(world.dt);

    // update sun
    float sunTime = quarter_pi<float>() + world.t * sunSpeed * 0.001f; // sun is fixed
    float sunDist = 100.f;
    sunDir = -normalize(vec3(cos(sunTime), 1, sin(sunTime))) * sunDist;

    // update the shadow map
    // update chunks
    auto chunks = renderer.visibleChunks(camera);
    for(auto& pair: chunks) {
        pair.second->update();
    }

    // remove empty chunks
    chunks.erase(std::remove_if(chunks.begin(), chunks.end(), [] (auto& pair) {
        return !pair.second->hasData();
    }), chunks.end());

    // prepare render
    glEnable(GL_DEPTH_TEST);
    #ifndef EMSCRIPTEN
      updateShadowMaps();
    #endif
    shader->activate();
    camera.activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set uniforms / textures
    updateUniforms(world.t);

    // draw skybox
    drawSkybox();

    // draw the terrain
    shader->bindTexture(TEXTURE_COLOR, texAtlas);
    #ifndef EMSCRIPTEN
        shadows.activate();
    #endif
    renderer.render(camera, chunks);

    // draw the entities
    drawEntities();
    
    // draw debug wireframe stuff
    if(debug)
        renderer.renderWireframe(camera);

    glDisable(GL_DEPTH_TEST);
    Component::draw();
}

std::unique_ptr<MonCraftScene> MonCraftScene::create(Viewport* vp) {
	auto scene = std::unique_ptr<MonCraftScene>(new MonCraftScene(vp));
	scene->initialize();
	return scene;
}