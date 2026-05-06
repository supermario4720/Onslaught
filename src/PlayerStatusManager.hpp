#pragma once
#include "Experience.hpp"
#include <vector>

class Player;

// Player stats stored here
struct PlayerStatus {
    float maxHealth = 100.f;
    float healthRecoverySpeed = 5.f;
    float maxStamina = 100.f;
    float staminaRecoverySpeed = 50.f;
    float attackDamage = 20.f;
    float movementSpeed = 80.f;
};

enum class PlayerStatType {
    MaxHealth = 0,
    HealthRecovery,
    MaxStamina,
    StaminaRecovery,
    AttackDamage,
    MovementSpeed,
    COUNT
};

struct PlayerProgression {
    PlayerStatus stats;
    int totalExperience = 0;
    int level = 1;
    int toNextLevel = 10;
    int statusPoints = 0;
};

class PlayerStatusManager {
public:
    PlayerStatusManager();

    void addExperience(int qty);

    void reset();

    void printExplvl() const;

    void levelUp();

    void setNextLevelReq();

    void updatePlayerStatus();

    void setPlayerPtr(std::weak_ptr<Player> ptr);

    bool spendStatusPoint(PlayerStatType stat);

    PlayerProgression getProgression() const;

private:
    std::weak_ptr<Player> playerPtr;
    // Player stat growth rates
    float healthIncrem;
    float healthRecoveryIncremRate;
    float staminaIncrem;
    float staminaRecoveryIncremRate;
    float damageIncrem;
    float moveSpeedIncrem;

    // Player stats managed here
    PlayerStatus stats;

    int totalExperience;
    int level;
    int toNextLevel;
    int statusPoints;
    int pointsPerLevel;
};
