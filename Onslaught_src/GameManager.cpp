#include "GameManager.hpp"
#include "Manager_UI_List.hpp"
#include "Camera.hpp"
#include "TitleScreen.hpp"
#include "GameUI.hpp"
#include "PauseUI.hpp"
#include "GameOverUI.hpp"
#include "VictoryUI.hpp"
#include "InventoryUI.hpp"
#include "TutorialUI.hpp"
#include "EntityManager.hpp"
#include "CollisionManager.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"

GameManager::GameManager(UIList& uiList, bool& firstTime, float screenWidth, float screenHeight)
    : uiList(uiList), entitiesInitialized(false), firstTime(firstTime), screenWidth(screenWidth), screenHeight(screenHeight) {}

void GameManager::update(float dt, GameStateManager::State gameState, GameStateManager& gsManager) {
    switch (gameState) {
        case GameStateManager::State::Title: {
            if (entitiesInitialized) entitiesInitialized = false;

            uiList.titleScreen.update(uiList.window);
            if (gsManager.getState() == GameStateManager::State::Playing && firstTime) {
                gsManager.setState(GameStateManager::State::Tutorial);
                firstTime = false;
            }
            break;
        }

        case GameStateManager::State::Tutorial: {
            // initialize (restart) entityManager on first frame of tutorial
            if (!entitiesInitialized) {
                EntityManager::getInstance().start(&uiList.camera, true);
                uiList.gameScreen.pauseButtonActive(false);
                uiList.gameScreen.setKeyboardVisibility(false);
                entitiesInitialized = true;
            }

            // update game state if inventory is closed
            if (!uiList.inventory.isInventoryOpen()) {
                EntityManager::getInstance().update(dt, uiList.window);
                EntityManager::getInstance().setInvUpdate(false);
                CollisionManager::getInstance().update(dt);
                uiList.gameScreen.update(dt, uiList.window, uiList.inventory);
                uiList.camera.follow(EntityManager::getInstance().getPlayerPos());
                uiList.tutorialScreen.update(dt, uiList.window);
            } else {
                EntityManager::getInstance().updateInventoryTextures(screenWidth, screenHeight);
                uiList.inventory.update(dt, uiList.window);
            }

            // check for game over
            if (!EntityManager::getInstance().isPlayerAlive() || !EntityManager::getInstance().isTownAlive()) {
                gsManager.setState(GameStateManager::State::GameOver);
            }

            if (gsManager.getState() != GameStateManager::State::Tutorial) {
                entitiesInitialized = false;
                gsManager.setState(GameStateManager::State::Playing);
            }

            break;
        }

        case GameStateManager::State::Playing: {
            // initialize (restart) entityManager on first frame of game
            if (!entitiesInitialized) {
                EntityManager::getInstance().start(&uiList.camera);
                uiList.gameScreen.pauseButtonActive(true);
                uiList.gameScreen.setKeyboardVisibility(true);
                entitiesInitialized = true;
            }

            // update game state if inventory is closed
            if (!uiList.inventory.isInventoryOpen()) {
                EntityManager::getInstance().update(dt, uiList.window);
                EntityManager::getInstance().setInvUpdate(false);
                CollisionManager::getInstance().update(dt);
                uiList.gameScreen.update(dt, uiList.window, uiList.inventory);
                uiList.camera.follow(EntityManager::getInstance().getPlayerPos());
            } else {
                EntityManager::getInstance().updateInventoryTextures(screenWidth, screenHeight);
                uiList.inventory.update(dt, uiList.window);
            }

            // check for game over
            if (!EntityManager::getInstance().isPlayerAlive() || !EntityManager::getInstance().isTownAlive()) {
                gsManager.setState(GameStateManager::State::GameOver);
            }
            break;
        }
        
        case GameStateManager::State::Pause: {
            uiList.pauseScreen.update(dt, uiList.window);
            break;
        }
        case GameStateManager::State::GameOver: {
            // set to false so entities reset next play
            // dont reset yet, so entities are still drawn in background
            if (entitiesInitialized) entitiesInitialized = false;

            uiList.gameOverScreen.update(dt, uiList.window);
            break;
        }
        case GameStateManager::State::Victory: {
            entitiesInitialized = false;
            uiList.winScreen.update(dt, uiList.window);
            break;
        }
        case GameStateManager::State::Exit: {
            entitiesInitialized = false;
            EntityManager::getInstance().reset();
            uiList.window.close();
            break;
        }
    }
}

void GameManager::render(GameStateManager::State gameState) {
    switch (gameState) {
        case GameStateManager::State::Title: {
            uiList.window.draw(uiList.background);
            uiList.titleScreen.render(uiList.window);
            break;
        }
        case GameStateManager::State::Playing: {
            uiList.window.clear(sf::Color(10, 10, 10, 255));
            uiList.window.draw(uiList.background);
            // adjusting background so it looks still
            const sf::Vector2u windowSize = uiList.window.getSize();
            uiList.background.setTextureRect(sf::IntRect(
                { static_cast<int>(uiList.camera.getPos().x),
                  static_cast<int>(uiList.camera.getPos().y) },
                { static_cast<int>(windowSize.x),
                  static_cast<int>(windowSize.y) }
            ));
            uiList.background.setPosition({ 0.f, 0.f });

            // render player and surroundings
            uiList.window.setView(uiList.camera.getView());
            EntityManager::getInstance().renderAlive(uiList.window);

            // render game UI
            uiList.window.setView(uiList.window.getDefaultView());
            uiList.gameScreen.render(uiList.window);
            if (uiList.inventory.isInventoryOpen()) uiList.inventory.render(uiList.window, EntityManager::getInstance().getPlayerInventory());
            break;
        }
        case GameStateManager::State::Tutorial: {
            uiList.window.clear(sf::Color(10, 10, 10, 255));
            uiList.window.draw(uiList.background);
            // adjusting background so it looks still
            const sf::Vector2u windowSize = uiList.window.getSize();
            uiList.background.setTextureRect(sf::IntRect(
                { static_cast<int>(uiList.camera.getPos().x),
                  static_cast<int>(uiList.camera.getPos().y) },
                { static_cast<int>(windowSize.x),
                  static_cast<int>(windowSize.y) }
            ));
            uiList.background.setPosition({ 0.f, 0.f });

            // render player and surroundings
            uiList.window.setView(uiList.camera.getView());
            EntityManager::getInstance().renderAlive(uiList.window);

            // render tutorial UI
            uiList.window.setView(uiList.window.getDefaultView());
            uiList.gameScreen.render(uiList.window);
            uiList.tutorialScreen.render(uiList.window);
            if (uiList.inventory.isInventoryOpen()) uiList.inventory.render(uiList.window, EntityManager::getInstance().getPlayerInventory());

            break;
        }
        case GameStateManager::State::Pause: {
            uiList.window.clear(sf::Color(10, 10, 10, 255));
            uiList.window.draw(uiList.background);
            // render player and surroundings
            uiList.window.setView(uiList.camera.getView());
            EntityManager::getInstance().renderAlive(uiList.window);

            uiList.window.setView(uiList.window.getDefaultView());
            uiList.gameScreen.render(uiList.window);
            // transparent sheet over game objects
            sf::RectangleShape fadeOverlay(sf::Vector2f(uiList.window.getSize()));
            fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
            uiList.window.draw(fadeOverlay);

            uiList.pauseScreen.render(uiList.window);
            break;
        }
        case GameStateManager::State::GameOver: {
            uiList.window.clear(sf::Color(10, 10, 10, 255));
            uiList.window.draw(uiList.background);
            // render player and surroundings
            uiList.window.setView(uiList.camera.getView());
            EntityManager::getInstance().renderAlive(uiList.window);

            uiList.window.setView(uiList.window.getDefaultView());
            uiList.gameScreen.render(uiList.window);
            // transparent sheet over game objects
            sf::RectangleShape fadeOverlay(sf::Vector2f(uiList.window.getSize()));
            fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
            uiList.window.draw(fadeOverlay);

            uiList.gameOverScreen.render(uiList.window);
            break;
        }
        case GameStateManager::State::Victory: {
            uiList.window.clear(sf::Color(10, 10, 10, 255));
            uiList.window.draw(uiList.background);
            // render player and surroundings
            uiList.window.setView(uiList.camera.getView());
            EntityManager::getInstance().renderAlive(uiList.window);

            uiList.window.setView(uiList.window.getDefaultView());
            uiList.gameScreen.render(uiList.window);
            // transparent sheet over game objects
            sf::RectangleShape fadeOverlay(sf::Vector2f(uiList.window.getSize()));
            fadeOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150 = semi-transparent black
            uiList.window.draw(fadeOverlay);

            uiList.winScreen.render(uiList.window);
            break;
        }
        case GameStateManager::State::Exit: {
            // No rendering for exit
            break;
        }
    }
}