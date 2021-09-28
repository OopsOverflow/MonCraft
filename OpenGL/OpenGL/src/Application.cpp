#include <iostream>
#include <memory>

#include "gl/ResourceManager.hpp"
#include "ui/ui.hpp"
#include "MonCraftScene.hpp"
#include "debug/Debug.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/common/Config.hpp"
#include "save/SaveManager.hpp"

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
}

std::unique_ptr<Server> createServer(Config const& config) {
    std::unique_ptr<Server> server;
    if (config.multiplayer) {
        server = std::make_unique<RealServer>(config.serverAddr, config.serverPort);
    } else {
        server = std::make_unique<ClientServer>();
    }
    return server;
}

float t = 0;
float dt = 0;
Viewport* pwindow;
MonCraftScene* pscene;
ui::Text* ptext_fps;
ui::Text* ptext_posPlayer;
ui::Text* ptext_players;
ui::Text* ptext_uid;
ui::Text* ptext_gameTime;
World* pworld;
Server* pserver;

void loop() {
  t += dt;

  pserver->update();

  pscene->drawFrame(t, dt);

  std::ostringstream text;
  text << "FPS : " << (int)(1.f / dt);
  ptext_fps->setText(text.str());

  text.str(""); // "clears" the string stream
  text << "Player Pos : " << std::fixed << std::setprecision(3) << pserver->getPlayer()->getPosition();
  ptext_posPlayer->setText(text.str());

  text.str(""); // "clears" the string stream
  text << "Players online : " << pworld->entities.count();
  ptext_players->setText(text.str());

  text.str(""); // "clears" the string stream
  text << "UID : " << pserver->getPlayer()->uid;
  ptext_uid->setText(text.str());

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
//
// int main(int argc, char* argv[]) {
//     std::cout << "---- Main ----" << std::endl;
//     Viewport window({1200, 800});


int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;
    Config config = SaveManager::getInst().getConfig();

    // game seed
    std::hash<std::string> hashString;
    auto seed = hashString(config.seed);
    std::srand(seed);
    std::cout << "seed : " << config.seed << " (" << seed << ")" << std::endl;

    Viewport window({800, 800});
    loadResources();
    window.createRoot();

    auto server = createServer(config);
    World& world = World::getInst();

    // UI stuff
    auto font_roboto = std::make_shared<const Font>("Roboto-Regular");
    auto font_vt323 = std::make_shared<const Font>("VT323-Regular");

    MonCraftScene scene(&window, server->getPlayer());
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

    ui::Button btn_fullscreen(&scene, "Fullscreen", font_vt323);
    btn_fullscreen.setAnchorX(ui::Anchor::END);
    btn_fullscreen.setPadding({15, 10});

    ui::Button btn_ping(&scene, "Ping", font_vt323);
    btn_ping.setPosition({0, 80}); // TODO: implement a box container
    btn_fullscreen.setPadding({15, 10});

    btn_vsync.onclick([&] { window.toggleVSync(); });
    btn_fullscreen.onclick([&] { window.toggleFullscreen(); });
    btn_ping.onclick([&] { server->ping(); });

    ui::Text text_posPlayer(&scene, "", font_vt323);
    text_posPlayer.setAnchorY(ui::Anchor::END);
    text_posPlayer.setFontSize(.5f);

    ui::Text text_gameTime(&scene, "", font_vt323);
    text_gameTime.setAnchorY(ui::Anchor::END);
    text_gameTime.setPosition(ivec2(0, -90)); // TODO: implement a box container
    text_gameTime.setFontSize(.5f);

    ui::Text text_players(&scene, "", font_vt323);
    text_players.setAnchorY(ui::Anchor::END);
    text_players.setPosition({0, -30}); // TODO: implement a box container
    text_players.setFontSize(.5f);

    ui::Text text_uid(&scene, "", font_vt323);
    text_uid.setAnchorY(ui::Anchor::END);
    text_uid.setPosition({0, -60}); // TODO: implement a box container
    text_uid.setFontSize(.5f);

    // main loop
    pwindow = &window;
    pscene = &scene;
    ptext_fps = &text_fps;
    ptext_posPlayer = &text_posPlayer;
    ptext_players = &text_players;
    ptext_uid = &text_uid;
    ptext_posPlayer = &text_posPlayer;
    ptext_gameTime = &text_gameTime;
    pworld = &world;
    pserver = server.get();

    #ifdef EMSCRIPTEN
      emscripten_set_main_loop(em_loop, 0, 1);
    #else
      for (dt = 0; window.beginFrame(dt); window.endFrame()) loop();
    #endif

    // for (float t = 0, dt = 0; window.beginFrame(dt); window.endFrame()) {
    //     t += dt;
    //
    //     server->update();
    //
    //     scene.drawFrame(t, dt);
    //
    //     std::ostringstream text;
    //     text << "FPS : " << (int)(1.f / dt);
    //     text_fps.setText(text.str());
    //
    //     text.str(""); // "clears" the string stream
    //     text << "Player Pos : " << std::fixed << std::setprecision(3) << server->getPlayer()->getPosition();
    //     text_posPlayer.setText(text.str());
    //
    //     text.str(""); // "clears" the string stream
    //     text << "Players online : " << world.entities.count();
    //     text_players.setText(text.str());
    //
    //     text.str(""); // "clears" the string stream
    //     text << "UID : " << server->getPlayer()->uid;
    //     text_uid.setText(text.str());
    //
    //     text.str(""); // "clears" the string stream
    //     text << "Game Time : " << std::fixed << std::setprecision(3) << t;
    //     text_gameTime.setText(text.str());
    // }

    ResourceManager::free();
    return 0;
}
