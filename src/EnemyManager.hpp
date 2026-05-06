#pragma once
#include "Enemy.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Camera;
class BuildManager;

class EnemyManager {
public:
    EnemyManager();

    void setCamera(Camera* ptr);

    // make it so that the function only needs to be called, and no params needed
        // building ID should be stored in the manager, with a select (update function updates the selected building on input)
    void spawnEnemy(sf::Vector2f position);

    void enemySpawnerUpdate(float dt);

	sf::Vector2f randomSpawnPosition();

    void update(float dt, const BuildingManager& buildManager);

    void render(sf::RenderWindow& window);

    void reset();

    const std::vector<std::shared_ptr<Enemy>>& getEnemyList() const;

    void setSpawnInterval(float min=2.f, float max=10.f);
    void setMaxSpawn(int max=15);

    int returnEnemyCount();

private:
    std::vector<std::shared_ptr<Enemy>> enemies;

    Camera* cameraPtr = nullptr;

    // replace with radius from enemy database if made later
    float enemySize = 30.f;

    int enemyCount = 0;
	int maxEnemies = 10;
	float spawnTimer = 0.f;
	float nextInterval = 1.f;
	float minSpawnInterval = 2.f;
	float maxSpawnInterval = 10.f;

};
