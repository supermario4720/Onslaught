#include "AudioManager.hpp"
#include <algorithm>
#include <cmath>

namespace {
	std::size_t musicIndex(MusicID id) {
		return static_cast<std::size_t>(id);
	}
}

AudioManager::~AudioManager() {
	clearManager();
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

void AudioManager::loadMusic(MusicID id, const std::string& filepath, float volume, bool loop) {
	if (!isValidMusicID(id)) return;

	auto music = std::make_unique<sf::Music>();
	if (!music->openFromFile(filepath)) {
		return;
	}

	music->setLooping(loop);
	music->setVolume(0.f);

	MusicTrack& track = musicTracks[musicIndex(id)];
	track.music = std::move(music);
	track.baseVolume = volume;
	track.currentVolume = 0.f;
	track.targetVolume = 0.f;
	track.fadeDuration = 1.f;
	track.loaded = true;
}

void AudioManager::playMusic(MusicID id, float fadeSeconds, bool restart) {
	if (id == MusicID::None) {
		stopMusic(fadeSeconds);
		return;
	}

	if (!isValidMusicID(id)) return;
	MusicTrack& nextTrack = musicTracks[musicIndex(id)];
	if (!nextTrack.loaded || !nextTrack.music) return;

	const float fadeDuration = std::max(0.05f, fadeSeconds);

	if (currentMusic == id && !restart) {
		nextTrack.fadeDuration = fadeDuration;
		if (nextTrack.music->getStatus() != sf::SoundSource::Status::Playing) {
			nextTrack.music->play();
		}
		refreshMusicTargets();
		return;
	}

	for (auto& track : musicTracks) {
		if (track.loaded) {
			track.fadeDuration = fadeDuration;
			track.targetVolume = 0.f;
		}
	}

	currentMusic = id;
	nextTrack.fadeDuration = fadeDuration;
	nextTrack.targetVolume = getEffectiveMusicVolume(nextTrack);

	if (restart) {
		nextTrack.music->stop();
		nextTrack.currentVolume = 0.f;
	}

	if (nextTrack.music->getStatus() != sf::SoundSource::Status::Playing) {
		nextTrack.music->play();
	}
}

void AudioManager::stopMusic(float fadeSeconds) {
	currentMusic = MusicID::None;
	const float fadeDuration = std::max(0.05f, fadeSeconds);

	for (auto& track : musicTracks) {
		if (track.loaded) {
			track.fadeDuration = fadeDuration;
			track.targetVolume = 0.f;
		}
	}
}

void AudioManager::setMusicMasterVolume(float volume) {
	musicMasterVolume = std::clamp(volume, 0.f, 100.f);
	refreshMusicTargets();
}

void AudioManager::setMusicDimmed(bool dimmed, float multiplier) {
	musicDimMultiplier = dimmed ? std::clamp(multiplier, 0.f, 1.f) : 1.f;
	refreshMusicTargets();
}

void AudioManager::update(float dt) {
	for (auto& track : musicTracks) {
		if (!track.loaded || !track.music) continue;

		float fadeDuration = std::max(0.05f, track.fadeDuration);
		float maxVolumeDelta = std::max(track.baseVolume, 1.f) * dt / fadeDuration;

		if (track.currentVolume < track.targetVolume) {
			track.currentVolume = std::min(track.currentVolume + maxVolumeDelta, track.targetVolume);
		}
		else if (track.currentVolume > track.targetVolume) {
			track.currentVolume = std::max(track.currentVolume - maxVolumeDelta, track.targetVolume);
		}

		track.music->setVolume(track.currentVolume);

		if (track.targetVolume <= 0.f &&
			track.currentVolume <= 0.01f &&
			track.music->getStatus() == sf::SoundSource::Status::Playing) {
			track.music->stop();
			track.currentVolume = 0.f;
			track.music->setVolume(0.f);
		}
	}
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

void AudioManager::loadAllMusic() {
	// Placeholder music for nowMorning_Loop.mp3
	loadMusic(MusicID::Title, "resources/music/2_23_AM_Loop.mp3", 34.f);
	loadMusic(MusicID::Game, "resources/music/Morning_Loop.mp3", 32.f);
	loadMusic(MusicID::Wave, "resources/music/Morning_Loop.mp3", 42.f);
	loadMusic(MusicID::Victory, "resources/music/2_23_AM_Loop.mp3", 34.f);
	loadMusic(MusicID::GameOver, "resources/music/2_23_AM_Loop.mp3", 30.f);
}

void AudioManager::clearManager() {
	stopMusic(0.05f);
	for (auto& track : musicTracks) {
		if (track.music) track.music->stop();
		track = MusicTrack();
	}
	buffers.clear();
	sounds.clear();
	currentMusic = MusicID::None;
	musicMasterVolume = 100.f;
	musicDimMultiplier = 1.f;
}

bool AudioManager::isValidMusicID(MusicID id) const {
	std::size_t index = musicIndex(id);
	return id != MusicID::None && index < musicTracks.size();
}

float AudioManager::getEffectiveMusicVolume(const MusicTrack& track) const {
	float volume = track.baseVolume * (musicMasterVolume / 100.f) * musicDimMultiplier;
	return std::clamp(volume, 0.f, 100.f);
}

void AudioManager::refreshMusicTargets() {
	for (std::size_t i = 0; i < musicTracks.size(); ++i) {
		MusicTrack& track = musicTracks[i];
		if (!track.loaded) continue;

		if (i == musicIndex(currentMusic)) {
			track.targetVolume = getEffectiveMusicVolume(track);
		}
		else {
			track.targetVolume = 0.f;
		}
	}
}
