#include "AudioManager.hpp"

AudioManager::~AudioManager() {
	buffers.clear();
	sounds.clear();
}

// Load sounds into the manager
void AudioManager::loadSound(const std::string& name, const std::string& filepath, float volume) {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filepath)) {
		// Handle file load error here
		return;
	}
	buffers[name] = {std::move(buffer), volume};
}

// Play one-shot sound (footstep, click, etc.)
void AudioManager::play(const std::string& name, bool canPlayMultiple) {
	auto it = buffers.find(name);
	if (it == buffers.end())
		return;

	const sf::SoundBuffer& buffer = it->second.first;
	float volume = it->second.second;

	if (!canPlayMultiple && isSoundPlaying(buffer)) {
		// Avoid starting duplicate overlapping sound
		return;
	}

	// Reuse an existing stopped sound if possible
	for (auto& s : sounds) {
		if (s.getStatus() == sf::Sound::Status::Stopped) {
			s.setBuffer(buffer);
			s.setVolume(volume);
			s.play();
			return;
		}
	}

	// Otherwise create a new one
	sf::Sound s(buffer);
	s.setVolume(volume);
	s.play();
	sounds.push_back(std::move(s));
}

// For advanced use: expose buffer directly if you want
const sf::SoundBuffer& AudioManager::getBuffer(const std::string& name) const {
	auto it = buffers.find(name);
	if (it != buffers.end())
		return it->second.first;
	throw std::runtime_error("SoundBuffer not found!");
}

bool AudioManager::isSoundPlaying(const sf::SoundBuffer& buffer) {
	for (const auto& s : sounds) {
		if (&s.getBuffer() == &buffer && s.getStatus() == sf::Sound::Status::Playing) {
			return true; // Already playing this sound buffer
		}
	}
	return false;
}



// Load all sounds somewhere at game start
void AudioManager::loadAllSounds() {
	loadSound("ButtonClick", "resources/sounds/01_chest_open_2.wav", 20.f);
	loadSound("ButtonHover", "resources/sounds/16_human_walk_stone_1.wav", 30.f);

	loadSound("playerWalkFootstep", "resources/sounds/04_sack_open_2.wav", 5.f);
	loadSound("playerDashFootstep", "resources/sounds/04_sack_open_3.wav", 5.f);
	loadSound("playerAttack", "resources/sounds/12_human_jump_3.wav", 20.f);
	loadSound("playerHurt", "resources/sounds/11_human_damage_3.wav", 10.f);
	loadSound("playerDeath", "resources/sounds/14_human_death_spin.wav", 20.f);

	//loadSound("enemyFootstep", "resources/sounds/footstep.wav", 15.f);
	loadSound("enemyAttack", "resources/sounds/18_orc_charge.wav", 7.f);
	loadSound("enemyHurt", "resources/sounds/17_orc_atk_sword_2.wav", 7.f);
	loadSound("enemyDeath", "resources/sounds/20_orc_special_atk.wav", 7.f);

	loadSound("townHurt", "resources/sounds/02_chest_close_2.wav", 15.f);
	loadSound("townDestroy", "resources/sounds/02_chest_close_2.wav", 15.f);
}

void AudioManager::clearManager() {
	buffers.clear();
	sounds.clear();
}