#include <iostream>
#include <memory>
#include <vector>
#include "gl/ResourceManager.hpp"
#include "gl/Viewport.hpp"
#include "interface/MonCraftScene.hpp"
#include "interface/MainMenu.hpp"




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
      ResourceManager::loadShader("simple", "simple.vert", "simple_emscripten.frag");
    #else
      ResourceManager::loadShader("simple", "simple.vert", "simple.frag");
    #endif
    ResourceManager::loadShader("skyBox", "skyBox.vert", "skyBox.frag");
    ResourceManager::loadShader("font",   "font.vert",   "font.frag");
    ResourceManager::loadShader("water",  "water.vert",  "water.frag");
    ResourceManager::loadShader("fog", "fog.vert", "fog.frag");
    ResourceManager::loadShader("pane", "pane.vert", "pane.frag");
    ResourceManager::loadShader("shadow", "shadow.vert", "shadow.frag");

    ResourceManager::loadTexture("atlas", "Texture_atlas");
    ResourceManager::loadTexture("character", "Character");
    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        ResourceManager::loadTexture("waterNormal" + std::to_string(i), filename);
    }

    ResourceManager::loadFont("roboto", "Roboto-Regular");
    ResourceManager::loadFont("vt323", "VT323-Regular");
}



int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;
    Viewport window({ 800, 800 });
    loadResources();
    window.createRoot();
    MainMenu mainMenu;
    std::unique_ptr<MonCraftScene> moncraftScene;

    window.getRoot()->addChild(&mainMenu);
    mainMenu.playButton->onclick([&] {
        moncraftScene = std::make_unique<MonCraftScene>(&window);
        window.getRoot()->removeChild(&mainMenu);
        window.getRoot()->addChild(moncraftScene.get());
    });


    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {
        World::getInst().t += dt;
        World::getInst().dt = dt;
    }

    ResourceManager::free();
    return 0;
}
