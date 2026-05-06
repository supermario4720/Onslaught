#include "PlayerStatusManager.hpp"
#include "Player.hpp"
#include <algorithm>
#include <iostream>

PlayerStatusManager::PlayerStatusManager()
: healthIncrem(10.f), healthRecoveryIncremRate(0.5f), staminaIncrem(10.f), staminaRecoveryIncremRate(0.5f),
damageIncrem(1.f), moveSpeedIncrem(5.f), stats(PlayerStatus()), statusPoints(0), pointsPerLevel(3)
{
    totalExperience = 0;
    level = 1;
    toNextLevel = 10;
}

void PlayerStatusManager::addExperience(int qty) {
    totalExperience += qty;

    int remainingQty = qty;

    while (remainingQty > 0) {
        int addedExp = std::min(remainingQty, toNextLevel);
        remainingQty -= addedExp;
        toNextLevel -= addedExp;
        if(toNextLevel == 0) levelUp();
    }
    updatePlayerStatus();
}

void PlayerStatusManager::reset() {
    totalExperience = 0;
    level = 1;
    statusPoints = 0;

    stats = PlayerStatus();

    healthIncrem = 10.f;
    healthRecoveryIncremRate = 0.5f;
    staminaIncrem = 10.f;
    staminaRecoveryIncremRate = 0.5f;
    damageIncrem = 1.f;
    moveSpeedIncrem = 5.f;
    
    toNextLevel = 10;
}

void PlayerStatusManager::printExplvl() const {
    std::cout << level << " : " <<  totalExperience << std::endl;
    // status
    std::cout << "Health : " <<  stats.maxHealth << std::endl;
    std::cout << "Health Recovery : " <<  stats.healthRecoverySpeed << std::endl;
    std::cout << "Stamina : " <<  stats.maxStamina << std::endl;
    std::cout << "Stamina Recovery : " <<  stats.staminaRecoverySpeed << std::endl;
    std::cout << "Attack Damage : " <<  stats.attackDamage << std::endl;
    std::cout << "Movement Speed : " <<  stats.movementSpeed << std::endl;
    std::cout << "Status Points : " << statusPoints << std::endl;
}


void PlayerStatusManager::levelUp() {
    level += 1;
    statusPoints += pointsPerLevel;
    setNextLevelReq();
    std::cout << "Level UP! " << std::endl;
    printExplvl();
}

void PlayerStatusManager::setNextLevelReq() {
    // make code or method to determine exp required to next level (e.g. lvl * 3exp)
    toNextLevel = 10;
}

void PlayerStatusManager::updatePlayerStatus() {
    if(playerPtr.expired()) return;

    auto player = playerPtr.lock();
    player -> setHealth(stats.maxHealth);
    player -> setHealthRecoverRate(stats.healthRecoverySpeed);
    player -> setStamina(stats.maxStamina);
    player -> setStaminaRevoverRate(stats.staminaRecoverySpeed);
    player -> setDamage(stats.attackDamage);
    player -> setMoveSpeed(stats.movementSpeed);
}

void PlayerStatusManager::setPlayerPtr(std::weak_ptr<Player> ptr) {
    playerPtr = ptr;
    updatePlayerStatus();
}

bool PlayerStatusManager::spendStatusPoint(PlayerStatType stat) {
    if (statusPoints <= 0) return false;

    switch (stat) {
        case PlayerStatType::MaxHealth:
            stats.maxHealth += healthIncrem;
            break;
        case PlayerStatType::HealthRecovery:
            stats.healthRecoverySpeed += healthRecoveryIncremRate;
            break;
        case PlayerStatType::MaxStamina:
            stats.maxStamina += staminaIncrem;
            break;
        case PlayerStatType::StaminaRecovery:
            stats.staminaRecoverySpeed += staminaRecoveryIncremRate;
            break;
        case PlayerStatType::AttackDamage:
            stats.attackDamage += damageIncrem;
            break;
        case PlayerStatType::MovementSpeed:
            stats.movementSpeed += moveSpeedIncrem;
            break;
        case PlayerStatType::COUNT:
            return false;
    }

    statusPoints--;
    updatePlayerStatus();
    return true;
}

PlayerProgression PlayerStatusManager::getProgression() const {
    PlayerProgression progression;
    progression.stats = stats;
    progression.totalExperience = totalExperience;
    progression.level = level;
    progression.toNextLevel = toNextLevel;
    progression.statusPoints = statusPoints;
    return progression;
}
