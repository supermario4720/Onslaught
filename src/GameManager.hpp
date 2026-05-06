#pragma once
#include <SFML/Graphics.hpp>
#include "GameStateManager.hpp"

class UIList;

class GameManager {
public:
    GameManager(UIList& uiList, bool& firstTime, float screenWidth, float screenHeight);

    void update(float dt, GameStateManager::State gameState, GameStateManager& gsManager);
    void render(GameStateManager::State gameState);

private:
    void updateMusic(float dt, GameStateManager::State gameState);

    UIList& uiList;
    bool entitiesInitialized;
    bool inTitleScreen;
    bool& firstTime;
    float screenWidth, screenHeight;
};
