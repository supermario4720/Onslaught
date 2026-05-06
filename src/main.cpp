#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "TitleScreen.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "AudioManager.hpp"

#include "GameManager.hpp"
#include "Manager_UI_List.hpp"

#include "Camera.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Object.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"
#include "TextureManager.hpp"
#include <vector>
#include <iostream>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>
#include <filesystem>
#endif





int main() {

    #ifdef __APPLE__
    {
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);

        std::vector<char> buffer(size);

        if (_NSGetExecutablePath(buffer.data(), &size) == 0)
        {
            std::filesystem::path exePath =
                std::filesystem::weakly_canonical(buffer.data());

            std::filesystem::path exeDir = exePath.parent_path();

            chdir(exeDir.c_str());

            std::cerr << "Changed working directory to: "
                      << std::filesystem::current_path()
                      << std::endl;
        }
    }
    #endif

    


    AudioManager::getInstance().loadAllSounds();
    AudioManager::getInstance().loadAllMusic();
    TextureManager::getInstance().loadAllTextures();
    // play noise at start so SFML loads sound properly
    AudioManager::getInstance().play("ButtonClick", 0.f);

    float screenWidth = 800.f;
    float screenHeight = 600.f;
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)}), "Game", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    Camera camera(window, screenWidth, screenHeight );

    sf::Clock clock;

    GameStateManager& gsManager = GameStateManager::getInstance();

    sf::Texture grassTexture("resources/grassTileTexture50.png");
    grassTexture.setRepeated(true);
    sf::Sprite background(grassTexture);
    background.setTextureRect(sf::IntRect({ 0, 0 }, { 800,600 }));

    UIList uiList(window, background, screenWidth, screenHeight);

    EntityManager& entityManager = EntityManager::getInstance();
    entityManager.start(&uiList.camera);
    bool firstTime = true;

    CollisionManager& collisionManager = CollisionManager::getInstance();
    InputManager& inputManager = InputManager::getInstance();
    inputManager.setCamera(uiList.camera);

    GameManager gameManager(uiList, firstTime, screenWidth, screenHeight);


    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        window.clear(sf::Color::Black);

        inputManager.update(window);

        GameStateManager::State gameState = gsManager.getState();

        gameManager.update(dt, gameState, gsManager);
        gameManager.render(gameState);

        window.display();
    }
    AudioManager::getInstance().clearManager();
    TextureManager::getInstance().clearManager();
    entityManager.clear();
    inputManager.clear();
    return 0;
}
