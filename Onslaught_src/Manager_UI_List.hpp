#pragma once
#include "Camera.hpp"
#include "TitleScreen.hpp"
#include "GameUI.hpp"
#include "PauseUI.hpp"
#include "GameOverUI.hpp"
#include "VictoryUI.hpp"
#include "InventoryUI.hpp"
#include "TutorialUI.hpp"

// file paths for fonts
static std::string fontPath = "resources/arial.ttf";
static std::string tutorialFontPath = "resources/NotoSansJP-VF.ttf";

// add all UI here for easier initialization, managing, and passing as arguments
struct UIList {
    sf::RenderWindow& window;
    sf::Font font;
    sf::Font tutorialTextFont;
    sf::Sprite& background;
    Camera camera;
    TitleScreen titleScreen; 
    GameUI gameScreen;
    PauseUI pauseScreen;
    GameOverUI gameOverScreen; 
    VictoryUI winScreen; 
    InventoryUI inventory;
    TutorialUI tutorialScreen; 

    UIList(sf::RenderWindow& w, sf::Sprite& bg, float screenWidth, float screenHeight)
    : window(w), font(fontPath), tutorialTextFont(tutorialFontPath), background(bg), camera(window, screenWidth, screenHeight), 
    titleScreen(font, window), gameScreen(font, window, &camera), pauseScreen(font, window), 
    gameOverScreen(font, window), winScreen(font, window), inventory(font, window), 
    tutorialScreen(font, tutorialTextFont, window)
    {
    }
};