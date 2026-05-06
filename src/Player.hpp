#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Hitbox.hpp"
#include "Entity.hpp"
#include "AnimationController.hpp"
#include "PlayerStatusManager.hpp"

class BuildingManager;
class PlayerStatusManager;

class Player : public Entity {
private:
    // set ptr to self for collision check
    std::weak_ptr<Player> selfPtr;
    sf::CircleShape shape;
    std::vector<std::shared_ptr<TriggerHitbox>> activeAttacks;

    PlayerStatusManager statusManager;

    float scale = 3.f;
    bool facingLeft = false;
    float spriteOffset = -25.f;
    bool keyboardInput = true;

    // player stats
    float healthRecoveryRate;
    float maxStamina;
    float currentStamina;
    float staminaRecoveryRate;

    float attackSpeed;
    float attackRange;
    float attackDamage;

    float currentSpeed;
    float movementSpeed;

    const float rollStaminaCost = 25.f;
    const float rollSpeedMultiplier = 3.f;
    const float rollDuration = 0.25f;
    float rollTimer = 0.f;
    sf::Vector2f rollDirection = { 0.f, 0.f };

    sf::Angle facing;
    sf::Vector2f movementVector;

    bool attackOnCD = false;
    bool attackHeld = false;
    bool dashing = false;
    bool dashHeld = false;
    bool rolling = false;
    // Check if build mode
    bool build = false;

    void startDodgeRoll(sf::Vector2f direction);
    void updateDodgeRoll(float dt);

public:
    Player();
    ~Player();

    // create and return pointer to instance
    static std::shared_ptr<Player> create();
    void initializeHitbox() override;
    void initializePtr(std::shared_ptr<Player>);

    void handleInput(BuildingManager& buildManager);
    void update(float deltaTime) override;
    // created player exclusive update function because it would need different parameters
    void updatePlayer(float dt, BuildingManager& buildManager);
    void updatePosition(sf::Vector2f movementVec, float dt) override;
    void onCollision(float damage, sf::Vector2f damageOrigin) override;
    void render(sf::RenderWindow& window) override;
    float getDirection() override;
    float getSize() override;

    void onAttack() override;
    const sf::Vector2f getPosition() const override;
    sf::CircleShape getShape() const;
    // return 2 floats (vector2f) of current/max hp
    sf::Vector2f getHealth() const;
    // return 2 floats (vector2f) of current/max stamina
    sf::Vector2f getStamina() const;

    void setHealth(float newHealth);
    void setHealthRecoverRate(float newRate);
    void setStamina(float newStamina);
    void setStaminaRevoverRate(float newRate);
    void setDamage(float newDamage);
    void setMoveSpeed(float newSpeed);

    void updateAnimationState(sf::Vector2f facing);
    bool isBuildMode() const;


    PlayerStatusManager& getStatusManager();
};
