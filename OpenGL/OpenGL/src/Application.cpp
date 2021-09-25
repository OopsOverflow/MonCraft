#include <iostream>
#include <memory>

#include "gl/ResourceManager.hpp"
#include "ui/ui.hpp"
#include "MonCraftScene.hpp"
#include "debug/Debug.hpp"
#include "multiplayer/client/ClientServer.hpp"
#include "multiplayer/client/RealServer.hpp"
#include "multiplayer/common/Config.hpp"

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

    ResourceManager::loadShader("simple", "simple.vert", "simple.frag");
    ResourceManager::loadShader("skyBox", "skyBox.vert", "skyBox.frag");
    ResourceManager::loadShader("font",   "font.vert",   "font.frag");
    ResourceManager::loadShader("water",  "water.vert",  "water.frag");
    ResourceManager::loadShader("fog", "fog.vert", "fog.frag");
    ResourceManager::loadShader("pane", "pane.vert", "pane.frag");

    ResourceManager::loadTexture("atlas", "Texture_atlas");
    ResourceManager::loadTexture("character", "Character");
    for (size_t i = 0; i < 30; i += 1) {
        std::string filename = "water/water_normal_" + std::to_string(i+1)+"_frame";
        ResourceManager::loadTexture("waterNormal" + std::to_string(i), filename);
    }
}

std::unique_ptr<Server> createServer() {
    std::unique_ptr<Server> server;
    if (!NetworkConfig::LOCAL) {
        server = std::make_unique<RealServer>(NetworkConfig::SERVER_ADDR, NetworkConfig::SERVER_PORT);
    } else {
        server = std::make_unique<ClientServer>();
    }
    return server;
}

int main(int argc, char* argv[]) {
    std::cout << "---- Main ----" << std::endl;

    Viewport window({800, 800});
    loadResources();
    window.createRoot();

    auto server = createServer();
    World& world = World::getInst();

    // game seed
    std::hash<std::string> hashString;
    std::srand(hashString("Moncraft"));

    float t = 0;

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

    ui::Button btn_gen(&scene, "Generation", font_vt323);
    btn_gen.setPosition({133, 0}); // TODO: implement a box container
    btn_gen.setPadding({15, 10});

    ui::Button btn_fullscreen(&scene, "Fullscreen", font_vt323);
    btn_fullscreen.setAnchorX(ui::Anchor::END);
    btn_fullscreen.setPadding({15, 10});

    ui::Button btn_ping(&scene, "Ping", font_vt323);
    btn_ping.setPosition({0, 80}); // TODO: implement a box container
    btn_fullscreen.setPadding({15, 10});

    btn_vsync.onclick([&] { window.toggleVSync(); });
    // btn_gen.onclick([&] { gen.toggleGeneration(); });
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
    for (float dt = 0; window.beginFrame(dt); window.endFrame()) {
        t += dt;

        server->update();

        scene.drawFrame(t, dt);

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
    return 0;
}
