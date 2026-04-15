#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    // Load sounds into the manager
    void loadSound(const std::string& name, const std::string& filepath, float volume = 50.f );

    // Play one-shot sound (footstep, click, etc.)
    void play(const std::string& name, bool canPlayMultiple = false);

    // For advanced use: expose buffer directly if you want
    const sf::SoundBuffer& getBuffer(const std::string& name) const;

    bool isSoundPlaying(const sf::SoundBuffer& buffer);

    void loadAllSounds();

    void clearManager();

private:
        // store sound buffer and volume
    std::unordered_map<std::string, std::pair<sf::SoundBuffer, float>> buffers;
    std::vector<sf::Sound> sounds;

    AudioManager() = default;
    ~AudioManager();
    // Delete copy/move so singleton can't be copied
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};