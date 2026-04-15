#include <SFML/Graphics.hpp>
#include "EntityManager.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Object.hpp"
#include "Town.hpp"
#include "Building.hpp"

#include "CollisionManager.hpp"
#include "GameStateManager.hpp"


EntityManager::EntityManager()
	: nextEntityID(0), playerPositionHolder({0.f,0.f}), isTutorial(false)
{
	itemManager = ItemManager();
	buildManager = BuildingManager();
	playerInventory = InventoryManager(15);
	enemyManager = EnemyManager();
	objectManager = ObjectManager();
}

EntityManager& EntityManager::getInstance() {
	static EntityManager instance;
	return instance;
}

void EntityManager::setWindow(sf::RenderWindow* _window) {
	window = _window;
}

void EntityManager::setCamera(Camera* cam) {
	camera = cam;
	enemyManager.setCamera(cam);
	objectManager.setCamera(cam);
}

void EntityManager::reset() {
	allEntities.clear();
	if(player) player->getStatusManager().reset();
	CollisionManager::getInstance().clear();
	enemyManager.reset();
	itemManager.reset();
	buildManager.reset();
	objectManager.reset();
	playerInventory.reset();
	
	nextEntityID = 0;
	score = 0;
	gameTime = 0.f;
	playerAlive = false;
	townAlive = false;
}

void EntityManager::clear() {
	reset();
	window = nullptr;
	camera = nullptr;
}

void EntityManager::start(Camera* cam, bool _isTutorial) {
	reset();
	isTutorial = _isTutorial;
	std::shared_ptr<Town> town = Town::createTown();
	town->setEntityID(0);
	townPtr = town;
	player = Player::create();
	player->setEntityID(1);
	nextEntityID = 2;

	allEntities.push_back(town);
	allEntities.push_back(player);
	playerAlive = true;
	townAlive = true;
	setCamera(cam);

	if(isTutorial) {
		enemyManager.setMaxSpawn(0);
		player->changeDestructability(false);
		town->changeDestructability(false);
		playerInventory.addItem(ItemID::Wood, 2);
		playerInventory.addItem(ItemID::Stone, 2);
	}
	else {
		enemyManager.setMaxSpawn();
		player->changeDestructability(true);
		town->changeDestructability(true);
		playerInventory.addItem(ItemID::Wood, 8);
		playerInventory.addItem(ItemID::Stone, 8);
	}
}

void EntityManager::update(float dt, sf::RenderWindow& _window) {
	auto town = townPtr.lock();

	//std::cout << "Running" << std::endl;
	// if player or town doesn't exist or is not alive, return
	if (!player || !player->checkAlive() ) {
		playerAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	if (!town || !town->checkAlive()) {
		townAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	playerPositionHolder = player->getPosition();
	itemManager.update(dt, playerPositionHolder, playerInventory, player->getStatusManager());
	buildManager.update(dt, playerPositionHolder, playerInventory, enemyManager, _window);
	enemyManager.update(dt, buildManager);

	// update spawn rate (wave) after 2 minutes in game
	if(gameTime > 120.f) {
		enemyManager.setSpawnInterval(1.2f, 3.6f);
		enemyManager.setMaxSpawn(30);
	}
	objectManager.update(dt);

	// update all entites
	player->updatePlayer(dt, buildManager);
	for (auto& ent : allEntities) {
		ent->update(dt);
	}
	// return if player or town died after update
	if (!player->checkAlive()) {
		playerAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	if (!town->checkAlive()) {
		townAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}

	// check if player near town
	town->checkPlayerNear(player->getPosition());
	if (town->isPlayerNear()) {
		// player->interactingWithTown();
		// std::cout << "Player is near Town" << std::endl;
		town->setColor(sf::Color(105, 105, 105));
	}
	else { town->setColor(sf::Color(155, 155, 155)); }

	for (auto it = allEntities.begin(); it != allEntities.end(); ) {
		if (!(*it)->checkAlive()) {
			it = allEntities.erase(it);   // deletes shared_ptr
		}
		else {
			++it;
		}
	}
	if(!isTutorial) gameTime += dt;
}

// must be called after updates
void EntityManager::renderAlive(sf::RenderWindow& _window) {
	for (auto& ent : allEntities) {
		ent->render(_window);
	}
	enemyManager.render(_window);
	itemManager.render(_window);
	buildManager.render(_window);
	objectManager.render(_window);
}

sf::Vector2f EntityManager::getPlayerPos() const {
	// if player ptr is null/empty
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 0.f };
	}
	else
	{
		return player->getPosition();
	}
}

bool EntityManager::isBuildMode() const {
	return player->isBuildMode();
}

int EntityManager::getScore() const {
	return score;
}

void EntityManager::incrementScore() {
	score++;
}

float EntityManager::getTime() const {
	return gameTime;
}

bool EntityManager::isPlayerAlive() const {
	return playerAlive;
}

bool EntityManager::isTownAlive() const {
	return townAlive;
}

sf::Vector2f EntityManager::getPlayerHealth() {
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 1.f };
	}
	else {
		return player->getHealth();
	}
}


sf::Vector2f EntityManager::getPlayerStamina() {
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 1.f };
	}
	else {
		return player->getStamina();
	}
}

	std::weak_ptr<Town> EntityManager::getTownPtr() const {
	return townPtr;
}

void EntityManager::spawnItems(ItemID id, const sf::Vector2f& pos, int qty) {
	itemManager.spawnItem(id, pos, qty);
}

void EntityManager::spawnExp(const sf::Vector2f& pos, int qty) {
	itemManager.spawnExp(pos, qty);
}

int EntityManager::getNextEntityID() {
	int id = nextEntityID;
	nextEntityID++;
	return id;
}

const InventoryManager& EntityManager::getPlayerInventory() const {
	return playerInventory;
}

void EntityManager::updateInventoryTextures(float screenW, float screenH) {
	if(!inventoryUpdated) playerInventory.updateSlotTextures(screenW, screenH);
	inventoryUpdated = true;
}

void EntityManager::setInvUpdate(bool updated) {
	inventoryUpdated = updated;
}

void EntityManager::setMaxEnemyCount(int maxCount) {
	enemyManager.setMaxSpawn(maxCount);
}
	
void EntityManager::setSpawnInterval(float min, float max) {
	enemyManager.setSpawnInterval(min, max);
}

void EntityManager::spawnEnemy() {
	enemyManager.spawnEnemy(enemyManager.randomSpawnPosition());
}

void EntityManager::spawnEnemy(sf::Vector2f position) {
	enemyManager.spawnEnemy(position);
}

bool EntityManager::doesEnemyExist() {
	int count = enemyManager.returnEnemyCount();
	if(count == 0) {
		return false;
	}
	return true;
}

bool EntityManager::doesBuildingExist() {
	return buildManager.doesBuildingExist();
}

bool EntityManager::doesExperienceExist() {
	return itemManager.doesExperienceExist();
}