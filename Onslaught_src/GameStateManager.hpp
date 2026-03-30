#pragma once

// class to make managing game states easier accross files
class GameStateManager {
public:
    enum State {
        Title,
        Playing,
        Pause,
        GameOver,
        Victory,
        Exit
    };

    static GameStateManager& getInstance();
    void setState(State newState); 
    GameStateManager::State getState() const;

private:
    State currentState = State::Title;
};