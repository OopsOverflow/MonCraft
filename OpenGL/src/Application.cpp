#include "rtc/rtc.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "gl/ResourceManager.hpp"
#include "gl/Viewport.hpp"
#include "interface/MonCraftScene.hpp"
#include "interface/MainMenu.hpp"
#include "interface/parametersMenu/ParametersMenu.hpp"
#include "save/ClientConfig.hpp"
#include "terrain/World.hpp"
#include "ui/Button.hpp"
#include "ui/Component.hpp"
#include "ui/Root.hpp"

#ifdef EMSCRIPTEN
    #include <emscripten.h>
#endif

using namespace glm;

void loadResources() {
    // Easy swap SkyBox
    // Tool : https://jaxry.github.io/panorama-to-cubemap/
    static const std::vector<std::string> skyboxDayFaces {
        "skybox/px.png",  // right
        "skybox/nx.png",  // left
        "skybox/py.png",  // top
        "skybox/ny.png",  // bottom
        "skybox/pz.png",  // front
        "skybox/nz.png"   // back
    };
    static const std::vector<std::string> skyboxNightFaces{
        "skybox/px(1).png",  // right
        "skybox/nx(1).png",  // left
        "skybox/py(1).png",  // top
        "skybox/ny(1).png",  // bottom
        "skybox/pz(1).png",  // front
        "skybox/nz(1).png"   // back
    };

    ResourceManager::loadCubeMap("skyboxDay", skyboxDayFaces);
    ResourceManager::loadCubeMap("skyboxNight", skyboxNightFaces);

    #ifdef EMSCRIPTEN
      ResourceManager::loadShader("simple", "simple_emscripten.vert", "simple_emscripten.frag");
    #else
      ResourceManager::loadShader("simple", "simple.vert", "simple.frag");
    #endif
    ResourceManager::loadShader("skyBox", "skyBox.vert", "skyBox.frag");
    ResourceManager::loadShader("font",   "font.vert",   "font.frag");
    ResourceManager::loadShader("water",  "water.vert",  "water.frag");
    ResourceManager::loadShader("pane", "pane.vert", "pane.frag");
    ResourceManager::loadShader("shadow", "shadow.vert", "shadow.frag");
    ResourceManager::loadShader("image", "image.vert", "image.frag");

    ResourceManager::loadTexture("atlas", "Texture_atlas");
    ResourceManager::loadTexture("character", "Character");
    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        ResourceManager::loadTexture("waterNormal" + std::to_string(i), filename);
    }
    ResourceManager::loadTexture("imageAtlas", "ImageAtlas");

    ResourceManager::loadFont("roboto", "Roboto-Regular");
    ResourceManager::loadFont("vt323", "VT323-Regular");
}

#ifdef EMSCRIPTEN
    Viewport* pwindow;
    void em_loop() {
        pwindow->beginFrame();
        pwindow->endFrame();
    }
#endif

void showView(Viewport& vp, std::unique_ptr<ui::Component> view) {
    auto children = vp.getRoot()->getChildren();
    if(children.size() > 0) vp.getRoot()->remove(children.at(0).get());
    vp.getRoot()->add(move(view));
}

void showSinglePlayer(Viewport& vp);
void showMultiPlayer(Viewport& vp);
void showParameters(Viewport& vp);
void showMainMenu(Viewport& vp);

void showSinglePlayer(Viewport& vp) {
    Config::getClientConfig().multiplayer = false;
    auto game = MonCraftScene::create(&vp);
    game->gameMenu->quitButton->onClick([&] { showMainMenu(vp); });
    game->gameMenu->continueButton->onClick([game = game.get(), menu = game->gameMenu.get(), &vp] { 
        game->remove(menu);
        game->makeActive();
        vp.captureMouse();
    });
    vp.captureMouse();
    game->makeActive();
    showView(vp, move(game));
    
}

void showMultiPlayer(Viewport& vp) {
    Config::getClientConfig().multiplayer = true;
    auto game = MonCraftScene::create(&vp);

    game->gameMenu->quitButton->onClick([&] { showMainMenu(vp); });
    game->gameMenu->continueButton->onClick([game = game.get(), menu = game->gameMenu.get(), &vp] { 
        game->remove(menu);
        game->makeActive();
        vp.captureMouse();
    });
    vp.captureMouse();
    game->makeActive();
    showView(vp, move(game));
    
}

void showParameters(Viewport& vp) {
    auto params = ParametersMenu::create();
    auto& config = Config::getClientConfig();
    params->quitButton->onClick([&] { showMainMenu(vp); });
    params->graphicsMenu->fullscreen->onRelease([&vp, &config, fullscreen = params->graphicsMenu->fullscreen.get()]{ 
        config.fullscreen = fullscreen->getChecked();
        vp.toggleFullscreen();
    });
    params->graphicsMenu->vsync->onRelease([&vp, &config, vsync = params->graphicsMenu->vsync.get()]{ 
        config.vsync = vsync->getChecked();
        vp.toggleVSync();
    });
    showView(vp, move(params));
}


void showMainMenu(Viewport& vp) {
    auto mainMenu = MainMenu::create();
    mainMenu->singleplayerButton->onClick([&]{ showSinglePlayer(vp); });
    mainMenu->multiplayerButton->onClick([&]{ showMultiPlayer(vp); });
    mainMenu->parameterButton->onClick([&]{ showParameters(vp); });
    mainMenu->quitButton->onClick([&] { vp.quit(); });
    showView(vp, move(mainMenu));
}


int main(int argc, char* argv[]) {
    spdlog::info("---- Main ----");
    spdlog::debug("Main thread is {}", std::this_thread::get_id());

    Viewport window({ 1200, 800 });
    std::unique_ptr<ui::Component> view;

    loadResources();
    window.createRoot();
    showMainMenu(window);

    #ifdef EMSCRIPTEN
        pwindow = &window;
        emscripten_set_main_loop(em_loop, 0, 1);
    #else
        while(window.beginFrame()) window.endFrame();
    #endif

    ResourceManager::free();
    return 0;
}
