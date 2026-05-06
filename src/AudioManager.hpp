#pragma once
#include <SFML/Audio.hpp>
#include "MusicID.hpp"
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

    void loadMusic(MusicID id, const std::string& filepath, float volume = 35.f, bool loop = true);

    void playMusic(MusicID id, float fadeSeconds = 1.5f, bool restart = false);

    void stopMusic(float fadeSeconds = 1.f);

    void setMusicMasterVolume(float volume);

    void setMusicDimmed(bool dimmed, float multiplier = 0.35f);

    void update(float dt);

    void loadAllSounds();

    void loadAllMusic();

    void clearManager();

private:
    struct MusicTrack {
        std::unique_ptr<sf::Music> music;
        float baseVolume = 35.f;
        float currentVolume = 0.f;
        float targetVolume = 0.f;
        float fadeDuration = 1.f;
        bool loaded = false;
    };

    static constexpr std::size_t musicCount = static_cast<std::size_t>(MusicID::COUNT);

    // store sound buffer and volume
    std::unordered_map<std::string, std::pair<sf::SoundBuffer, float>> buffers;
    std::vector<sf::Sound> sounds;
    std::array<MusicTrack, musicCount> musicTracks;

    MusicID currentMusic = MusicID::None;
    float musicMasterVolume = 15.f;
    float musicDimMultiplier = 1.f;

    AudioManager() = default;
    ~AudioManager();
    // Delete copy/move so singleton can't be copied
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool isValidMusicID(MusicID id) const;

    float getEffectiveMusicVolume(const MusicTrack& track) const;

    void refreshMusicTargets();
};
