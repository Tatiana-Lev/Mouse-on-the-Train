#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Monster.h"
#include "Leaderboard.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View gameView;
    sf::Vector2u windowSize;

    std::vector<sf::Texture> backgroundTextures;
    sf::Sprite backgroundSprite;

    Player player;
    Monster monster1;
    Monster monster2;
    Monster monster3;
    Monster monster4;

    int currentLocation;
    bool gameOver;
    bool victory;
    bool doorOpen;
    bool canAttack;
    float attackCooldown;
    float doorCooldown = 0.0f;
    bool fontLoaded = false;

    sf::Font font;
    sf::Text text;
    sf::Text notificationText;
    std::string currentNotification;
    float notificationTimer = 0.0f;

    float marmaladeCooldown = 0.0f;

    void handleInput(float deltaTime);
    void updateMonsters(float deltaTime);
    void checkDoorInteraction();
    void updateAttackCooldown(float deltaTime);
    void updateMarmaladeCooldown(float deltaTime);
    Monster* getCurrentMonster();
    void drawGround();
    void drawDoor();
    void drawUI();
    void drawHealthBar();
    void loadFont();
    void showLocationInfo();
    void updateBackground();
    void showNotification(const std::string& msg);
    void drawNotification();

    Leaderboard leaderboard;
    float gameTimer = 0.0f;
    bool timerRunning = true;
    bool victoryScreen = false;
    bool showLeaderboard = false;
    std::string playerName;
    sf::String nameInput;

    void drawVictoryInputScreen();
    void handleVictoryInput(sf::Event& event);

    bool showStartScreen;
    void drawStartScreen();

public:
    Game();
    ~Game();
    void run();
};