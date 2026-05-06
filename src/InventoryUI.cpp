#include <SFML/Graphics.hpp>
#include "InventoryUI.hpp"
#include "InventoryManager.hpp"
#include "EntityManager.hpp"
#include <iomanip>
#include <sstream>

namespace {
std::string formatFloat(float value, int precision = 1) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}
}

InventoryUI::InventoryUI(sf::Font& font, const sf::RenderWindow& window)
    : inventoryText(font), statusText(font), progressionText(font),
    resumeButton({ 50.f, 50.f }, { 0.f, 0.f }, " X ", font), inventoryOpen(false)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    // the background dimmed out
    background.setSize({ screenWidth, screenHeight });
    background.setFillColor(sf::Color(0, 0, 0, 50));
    background.setPosition( {0, 0} );

    // Inventory UI background / base
    inventoryBase.setSize({ screenWidth*3.f/4.f, screenHeight*3.f/4.f });
    inventoryBase.setFillColor(sf::Color(30, 30, 30, 220));
    inventoryBase.setOrigin({ screenWidth*3.f/8.f, screenHeight*3.f/8.f });
    inventoryBase.setPosition( { screenWidth/2.f, screenHeight/2.f } );

    inventoryText = sf::Text(font);
    inventoryText.setCharacterSize(24);
    inventoryText.setString("Inventory");
    inventoryText.setFillColor(sf::Color(255, 255, 255, 200));
    auto bounds = inventoryText.getLocalBounds();
    inventoryText.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    inventoryText.setPosition( { screenWidth/5.f, screenHeight/7.f } );

    resumeButton.setPosition({ screenWidth*7.f/8.f - 20.f, screenHeight/8.f + 20.f });  // Top-right
    resumeButton.setCharSize(32);

    statusText.setCharacterSize(24);
    statusText.setString("Status");
    statusText.setFillColor(sf::Color(255, 255, 255, 200));
    statusText.setPosition({ screenWidth * 0.58f, screenHeight / 7.f });

    progressionText.setCharacterSize(18);
    progressionText.setFillColor(sf::Color(230, 230, 230, 220));
    progressionText.setPosition({ screenWidth * 0.58f, screenHeight / 7.f + 35.f });

    const int statCount = static_cast<int>(PlayerStatType::COUNT);
    float statStartY = screenHeight / 7.f + 140.f;
    for (int i = 0; i < statCount; ++i) {
        sf::Text statLine(font);
        statLine.setCharacterSize(17);
        statLine.setFillColor(sf::Color(235, 235, 235, 220));
        statLine.setPosition({ screenWidth * 0.58f, statStartY + i * 36.f });
        statTexts.push_back(statLine);

        statButtons.emplace_back(sf::Vector2f{ 32.f, 32.f },
                                 sf::Vector2f{ screenWidth * 0.83f, statStartY + i * 36.f + 10.f },
                                 "+",
                                 font);
        statButtons.back().setCharSize(22);
        statButtons.back().setTextOffset(8.f);
    }

    updateStatusText();
}

InventoryUI::~InventoryUI() {
}

void InventoryUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& entityManager = EntityManager::getInstance();

    updateStatusText();
    
    bool resumeHover = resumeButton.isMouseOver(window);
    resumeButton.setHover(resumeHover);

    if (resumeButton.isClicked(window)) {
        inventoryOpen = false;
    }

    const bool hasStatusPoints = entityManager.getPlayerProgression().statusPoints > 0;
    for (std::size_t i = 0; i < statButtons.size(); ++i) {
        if (!hasStatusPoints) {
            statButtons[i].setHover(false);
            continue;
        }

        bool hover = statButtons[i].isMouseOver(window);
        statButtons[i].setHover(hover);

        if (statButtons[i].isClicked(window)) {
            entityManager.spendPlayerStatusPoint(static_cast<PlayerStatType>(i));
            updateStatusText();
        }
    }
}

void InventoryUI::render(sf::RenderWindow& window, const InventoryManager& inventoryManager) {
    window.draw(background);
    window.draw(inventoryBase);
    window.draw(inventoryText);
    window.draw(statusText);
    window.draw(progressionText);
    resumeButton.render(window);
    
    const std::vector<InventorySlot>& inventory = inventoryManager.getInventory();
    for(auto slot : inventory) {
        if(slot.id == ItemID::None) window.draw(slot.emptySlot);
        else {
            window.draw(slot.sprite);
            window.draw(slot.qtyText);
        }
        
    }

    for (const auto& statLine : statTexts) {
        window.draw(statLine);
    }
    for (const auto& button : statButtons) {
        button.render(window);
    }
}

void InventoryUI::setVisibility(bool open) {
    inventoryOpen = open;
}

bool InventoryUI::isInventoryOpen() const {
    return inventoryOpen;
}

void InventoryUI::updateStatusText() {
    PlayerProgression progression = EntityManager::getInstance().getPlayerProgression();
    const PlayerStatus& stats = progression.stats;

    std::stringstream progress;
    progress << "Level " << progression.level << "\n"
             << "XP: " << progression.totalExperience << "\n"
             << "Next: " << progression.toNextLevel << "\n"
             << "Points: " << progression.statusPoints;
    progressionText.setString(progress.str());

    if (statTexts.size() < static_cast<std::size_t>(PlayerStatType::COUNT)) return;

    statTexts[static_cast<std::size_t>(PlayerStatType::MaxHealth)]
        .setString("Max HP: " + formatFloat(stats.maxHealth, 0));
    statTexts[static_cast<std::size_t>(PlayerStatType::HealthRecovery)]
        .setString("HP Regen: " + formatFloat(stats.healthRecoverySpeed));
    statTexts[static_cast<std::size_t>(PlayerStatType::MaxStamina)]
        .setString("Max Stamina: " + formatFloat(stats.maxStamina, 0));
    statTexts[static_cast<std::size_t>(PlayerStatType::StaminaRecovery)]
        .setString("Stam Regen: " + formatFloat(stats.staminaRecoverySpeed));
    statTexts[static_cast<std::size_t>(PlayerStatType::AttackDamage)]
        .setString("Damage: " + formatFloat(stats.attackDamage));
    statTexts[static_cast<std::size_t>(PlayerStatType::MovementSpeed)]
        .setString("Move Speed: " + formatFloat(stats.movementSpeed));
}
