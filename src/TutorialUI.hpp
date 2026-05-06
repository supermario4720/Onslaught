#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

class TutorialUI {
public:
    TutorialUI(sf::Font& font, sf::Font& tutorialTextFont, sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void controlStep();

    void buildStep();

    void enemyStep();

    void render(sf::RenderWindow& window);

    bool isFinished() const;

private:
    sf::Text tutorialText;
    Button nextButton;
    Button skipButton;

    float screenWidth;
    float screenHeight;

    int currentTutorialPhase = 0;
    int currentStep = 0;
    bool finished = false;
    bool enemySpawned = false;
};