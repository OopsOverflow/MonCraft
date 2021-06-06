#include <iostream>
#include <memory>

#include "gl/ResourceManager.hpp"
#include "ui/ui.hpp"
#include "MonCraftScene.hpp"
#include "debug/Debug.hpp"

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

int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;
    Viewport window({800, 800});
    loadResources();
    window.createRoot();

    // game seed
    std::hash<std::string> hashString;
    std::srand(hashString("Moncraft"));

    float t = 0;

    // UI stuff
    auto font_roboto = std::make_shared<const Font>("Roboto-Regular");
    auto font_vt323 = std::make_shared<const Font>("VT323-Regular");

    MonCraftScene scene(&window);
    scene.setPadding({10, 10});

    ui::Pane pane_fps(&scene);
    pane_fps.setColor({ 1.f, 1.f, 1.f, 0.5f });
    pane_fps.setPosition({ -10, -10 });
    pane_fps.setAnchorY(ui::Anchor::END);
    pane_fps.setAnchorX(ui::Anchor::END);
    pane_fps.setPadding({ 10, 10 });
    pane_fps.setSize({ 300, 10 });

    ui::Text text_fps(&pane_fps, "hello", font_vt323);
    text_fps.setFontSize(2.f);
    text_fps.setColor({ 0.8f, 0.7f, 0.0f, 1.f });

    ui::Button btn_vsync(&scene, "VSync", font_vt323);
    btn_vsync.setPadding({15, 10});

    ui::Button btn_gen(&scene, "Generation", font_vt323);
    btn_gen.setPosition({133, 0}); // TODO: implement a box container
    btn_gen.setPadding({15, 10});

    ui::Button btn_fog(&scene, "Fog", font_vt323);
    btn_fog.setPosition({360, 0}); // TODO: implement a box container
    btn_fog.setPadding({15, 10});

    btn_vsync.onclick([&] { window.toggleVSync(); });
    btn_gen.onclick([&] { scene.terrain.toggleGeneration(); });
    btn_fog.onclick([&] { scene.fogEnabled = !scene.fogEnabled; });

    // main loop
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {
        t += dt;

        scene.drawFrame(t, dt);

        std::string text = "FPS : " + std::to_string((int)(1.f / dt));
        text_fps.setText(text);
    }

    ResourceManager::free();
    return 0;
}
