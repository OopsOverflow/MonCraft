#include <iostream>
#include <memory>

#include "gl/ResourceManager.hpp"
#include "ui/ui.hpp"
#include "MonCraftScene.hpp"
#include "debug/Debug.hpp"

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
    ResourceManager::loadShader("simple", "simple.vert", "simple_emscripten.frag");
    #else
    ResourceManager::loadShader("simple", "simple.vert", "simple.frag");
    #endif
    ResourceManager::loadShader("skyBox", "skyBox.vert", "skyBox.frag");
    ResourceManager::loadShader("font",   "font.vert",   "font.frag");
    ResourceManager::loadShader("water",   "water.vert",   "water.frag");
    ResourceManager::loadShader("fog", "fog.vert", "fog.frag");
    ResourceManager::loadShader("pane", "pane.vert", "pane.frag");
    ResourceManager::loadShader("shadow", "shadow.vert", "shadow.frag");

    ResourceManager::loadTexture("atlas", "Texture_atlas");
    ResourceManager::loadTexture("character", "Character");
    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        ResourceManager::loadTexture("waterNormal" + std::to_string(i), filename);
    }
}

float t = 0;
float dt = 0;
Viewport* pwindow;
MonCraftScene* pscene;
ui::Text* ptext_fps;
ui::Text* ptext_posPlayer;
ui::Text* ptext_gameTime;

void loop() {
  t += dt;

  pscene->drawFrame(t, dt);

  std::ostringstream text;
  text << "FPS : " << (int)(1.f / dt);
  ptext_fps->setText(text.str());

  text.str(""); // "clears" the string stream
  text << "Player Pos : " << std::fixed << std::setprecision(3) << pscene->character.getPosition();
  ptext_posPlayer->setText(text.str());

  text.str(""); // "clears" the string stream
  text << "Game Time : " << std::fixed << std::setprecision(3) << t;
  ptext_gameTime->setText(text.str());
}

#ifdef EMSCRIPTEN
void em_loop() {
  pwindow->beginFrame(dt);
  loop();
  pwindow->endFrame();
}
#endif

int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;
    Viewport window({1200, 800});
    loadResources();
    window.createRoot();

    // game seed
    std::hash<std::string> hashString;
    std::srand(hashString("Moncraft"));

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

    ui::Text text_fps(&pane_fps, "", font_vt323);
    text_fps.setFontSize(2.f);
    text_fps.setColor({ 0.8f, 0.7f, 0.0f, 1.f });

    ui::Button btn_vsync(&scene, "VSync", font_vt323);
    btn_vsync.setPadding({15, 10});

    ui::Button btn_gen(&scene, "Generation", font_vt323);
    btn_gen.setPosition({133, 0}); // TODO: implement a box container
    btn_gen.setPadding({15, 10});
    /*
    ui::Button btn_fog(&scene, "Fog", font_vt323);
    btn_fog.setPosition({360, 0}); // TODO: implement a box container
    btn_fog.setPadding({15, 10});
    */
    ui::Button btn_fullscreen(&scene, "Fullscreen", font_vt323);
    btn_fullscreen.setAnchorX(ui::Anchor::END);
    btn_fullscreen.setPadding({15, 10});

    btn_vsync.onclick([&] { window.toggleVSync(); });
    btn_gen.onclick([&] { scene.terrain.toggleGeneration(); });
    //btn_fog.onclick([&] { scene.fogEnabled = !scene.fogEnabled; });
    btn_fullscreen.onclick([&] { window.toggleFullscreen(); });

    ui::Text text_posPlayer(&scene, "", font_vt323);
    text_posPlayer.setAnchorY(ui::Anchor::END);
    text_posPlayer.setFontSize(.5f);

    ui::Text text_gameTime(&scene, "", font_vt323);
    text_gameTime.setAnchorY(ui::Anchor::END);
    text_gameTime.setPosition(ivec2(0, -40));
    text_gameTime.setFontSize(.5f);

    // main loop
    pwindow = &window;
    pscene = &scene;
    ptext_fps = &text_fps;
    ptext_posPlayer = &text_posPlayer;
    ptext_gameTime = &text_gameTime;

    #ifdef EMSCRIPTEN
      emscripten_set_main_loop(em_loop, 0, 1);
    #else
    for (dt = 0; window.beginFrame(dt); window.endFrame()) {
      loop();
    }
    #endif

    ResourceManager::free();
    return 0;
}
