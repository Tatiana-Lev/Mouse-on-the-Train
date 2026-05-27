#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Mouse on the Train", sf::Style::Default),
    windowSize(1920, 1080),
    monster1("Conductor", 45, 12, "conductor.png", 85.0f),
    monster2("Magician", 60, 15, "magician.png", 80.0f),
    monster3("Speculator", 50, 10, "speculator.png", 95.0f),
    monster4("Final Boss", 100, 20, "boss.png", 70.0f),
    currentLocation(0),
    gameOver(false),
    victory(false),
    doorOpen(true),
    canAttack(true),
    attackCooldown(0),
    showStartScreen(true) {

    loadFont();

    gameView.setSize(800, 500);
    gameView.setCenter(400, 250);
    window.setView(gameView);

    std::string backgrounds[] = {
        "station.png",
        "car0.png",
        "car1.png",
        "car2.png",
        "car3.png",
        "car4.png",
        "station_nn.png"
    };

    backgroundTextures.resize(7);
    for (int i = 0; i < 7; i++) {
        if (backgroundTextures[i].loadFromFile(backgrounds[i])) {
        }
        else {
            backgroundTextures[i].create(800, 500);
        }
    }

    backgroundSprite.setTexture(backgroundTextures[0]);
    backgroundSprite.setScale(
        800.0f / backgroundTextures[0].getSize().x,
        500.0f / backgroundTextures[0].getSize().y
    );

    player.setTexture("mouse.png");
}

Game::~Game() {}

void Game::loadFont() {
    if (font.loadFromFile("font.ttf")) {
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        notificationText.setFont(font);
        notificationText.setCharacterSize(18);
        notificationText.setFillColor(sf::Color::Yellow);

        fontLoaded = true;
        std::cout << "[OK] Font loaded successfully.\n";
    }
    else {
        fontLoaded = false;
        std::cout << "[ERROR] Failed to load font.ttf\n";
    }
}

void Game::showNotification(const std::string& msg) {
    currentNotification = msg;
    notificationTimer = 2.0f;
}

void Game::drawNotification() {
    if (notificationTimer > 0.0f && !currentNotification.empty()) {
        notificationText.setString(currentNotification);
        notificationText.setPosition(300, 420);
        window.draw(notificationText);
    }
}

Monster* Game::getCurrentMonster() {
    if (currentLocation == 2 && monster1.isAlive()) return &monster1;
    if (currentLocation == 3 && monster2.isAlive()) return &monster2;
    if (currentLocation == 4 && monster3.isAlive()) return &monster3;
    if (currentLocation == 5 && monster4.isAlive()) return &monster4;
    return nullptr;
}

void Game::handleInput(float deltaTime) {
    if (gameOver || victory || victoryScreen || showLeaderboard) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.moveLeft(deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.moveRight(deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) player.jump();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && marmaladeCooldown <= 0.0f) {
        int beforeMarmalade = player.getMarmalade();
        int beforeHp = player.getHp();
        player.useMarmalade();

        if (player.getMarmalade() < beforeMarmalade) {
            int healed = player.getHp() - beforeHp;
            showNotification("Used marmalade! +" + std::to_string(healed) + " HP");
            marmaladeCooldown = 0.8f;
        }
        else if (beforeMarmalade == 0) {
            showNotification("No marmalade left!");
            marmaladeCooldown = 0.5f;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && canAttack) {
        canAttack = false;
        attackCooldown = 0.5f;
        player.attack();
        Monster* monster = getCurrentMonster();
        if (monster != nullptr) {
            float distX = std::abs(player.getX() - monster->getX());
            float distY = std::abs(player.getY() - monster->getY());
            if (distX < 70 && distY < 60) {
                monster->takeDamage(15);
                if (!monster->isAlive()) {
                    doorOpen = true;
                    player.addMarmalade();
                    showNotification("Monster defeated! +1 marmalade!");
                    std::cout << "Door opened! Press F to proceed.\n";
                }
                else {
                    showNotification("Hit! Dealt 15 damage!");
                }
            }
            else {
                showNotification("Too far to attack!");
                std::cout << "Too far to attack!\n";
            }
        }
        else if (currentLocation >= 2 && currentLocation <= 5) {
            showNotification("No monster here!");
        }
    }
    checkDoorInteraction();
}

void Game::checkDoorInteraction() {
    if (doorCooldown > 0.0f) {
        doorCooldown -= 0.016f;
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && doorOpen) {
        if (player.getX() + player.getBounds().width > 700) {
            currentLocation++;
            player.resetPosition();
            doorOpen = false;
            doorCooldown = 0.5f;

            updateBackground();

            std::cout << "Moving to location " << currentLocation << "\n";

            if (currentLocation == 0 || currentLocation == 1 || currentLocation == 6) {
                doorOpen = true;
            }
            if (currentLocation == 6) {
                victory = true;
                timerRunning = false;
                victoryScreen = true;
                showNotification("VICTORY! You reached the station!");
                std::cout << "VICTORY! Time: " << (int)gameTimer << "s\n";
            }
        }
        else {
            showNotification("Go to the door (right side)");
        }
    }
}

void Game::updateBackground() {
    if (currentLocation >= 0 && currentLocation < (int)backgroundTextures.size()) {
        backgroundSprite.setTexture(backgroundTextures[currentLocation]);
        backgroundSprite.setScale(
            800.0f / backgroundTextures[currentLocation].getSize().x,
            500.0f / backgroundTextures[currentLocation].getSize().y
        );
    }
}

void Game::drawDoor() {
    sf::RectangleShape door(sf::Vector2f(40, 80));

    if (doorOpen) {
        door.setFillColor(sf::Color::Green);
    }
    else {
        door.setFillColor(sf::Color(139, 69, 19));
    }

    door.setPosition(730, 320);
    window.draw(door);
}

void Game::updateMonsters(float deltaTime) {
    Monster* monster = getCurrentMonster();
    if (monster != nullptr) {
        monster->update(deltaTime, player.getX());

        if (monster->isCloseTo(player.getX(), player.getY()) && monster->readyToAttack()) {
            int beforeHp = player.getHp();
            player.takeDamage(monster->getDamage());
            showNotification("Took " + std::to_string(monster->getDamage()) + " damage!");

            float kbDir = (player.getX() < monster->getX()) ? -1.0f : 1.0f;
            player.setX(player.getX() + kbDir * 70.0f);

            if (player.getHp() <= 0) {
                gameOver = true;
                player.setCanMove(false);
                timerRunning = false;
                showNotification("GAME OVER!");
                std::cout << "GAME OVER!\n";
            }
        }
    }
}

void Game::updateAttackCooldown(float deltaTime) {
    if (!canAttack) {
        attackCooldown -= deltaTime;
        if (attackCooldown <= 0) {
            canAttack = true;
        }
    }
}

void Game::updateMarmaladeCooldown(float deltaTime) {
    if (marmaladeCooldown > 0.0f) {
        marmaladeCooldown -= deltaTime;
    }
}

void Game::drawHealthBar() {
    sf::RectangleShape healthBg(sf::Vector2f(200, 20));
    healthBg.setFillColor(sf::Color::Red);
    healthBg.setPosition(10, 10);
    window.draw(healthBg);

    sf::RectangleShape healthBar(sf::Vector2f(200 * player.getHealth().getPercent(), 20));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(10, 10);
    window.draw(healthBar);
}

void Game::drawGround() {
    sf::RectangleShape ground(sf::Vector2f(800, 5));
    ground.setFillColor(sf::Color(100, 70, 40));
    ground.setPosition(0, 400);
    window.draw(ground);
}

void Game::showLocationInfo() {
    if (!fontLoaded) return;

    switch (currentLocation) {
    case 0:
        text.setString("MUROM STATION: A/D - move, SPACE - jump");
        text.setPosition(150, 100); window.draw(text);
        text.setString("Press F at the door to enter the train");
        text.setPosition(150, 130); window.draw(text);
        break;
    case 1:
        text.setString("CAR 0: Proceed to the next car");
        text.setPosition(250, 100); window.draw(text);
        break;
    case 2:
        if (monster1.isAlive()) {
            text.setString("CAR 1: Conductor! Press Q to attack!");
            text.setPosition(200, 100); window.draw(text);
        }
        break;
    case 3:
        if (monster2.isAlive()) {
            text.setString("CAR 2: Magician! R - heal with marmalade");
            text.setPosition(200, 100); window.draw(text);
        }
        break;
    case 4:
        if (monster3.isAlive()) {
            text.setString("CAR 3: Normal monster");
            text.setPosition(250, 100); window.draw(text);
        }
        break;
    case 5:
        if (monster4.isAlive()) {
            text.setString("CAR 4: FINAL BOSS!");
            text.setPosition(250, 100); window.draw(text);
        }
        break;
    case 6:
        text.setString("NIZHNY NOVGOROD STATION!");
        text.setPosition(250, 100); window.draw(text);
        text.setString("Fun fact: City founded in 1221");
        text.setPosition(200, 130); window.draw(text);
        break;
    }
}

void Game::drawUI() {
    drawHealthBar();
    if (!fontLoaded) return;

    text.setString("HP: " + std::to_string(player.getHp()) + "/100");
    text.setPosition(10, 35); window.draw(text);

    text.setString("Marmalade: " + std::to_string(player.getMarmalade()));
    text.setPosition(220, 10); window.draw(text);

    if (!gameOver && !victory && !victoryScreen && !showLeaderboard) {
        text.setString("Time: " + std::to_string((int)gameTimer) + "s");
        text.setPosition(680, 10);
        window.draw(text);
    }

    showLocationInfo();
    drawNotification();

    Monster* monster = getCurrentMonster();
    if (monster != nullptr && currentLocation >= 2 && currentLocation <= 5) {
        text.setString(monster->getName() + ": " + std::to_string(monster->getHp()) + "/" + std::to_string(monster->getMaxHp()));
        text.setPosition(monster->getX() - 30, 350);
        window.draw(text);
    }

    if (gameOver) {
        text.setString("DEFEAT! Press ESC to exit");
        text.setPosition(250, 200);
        text.setCharacterSize(30);
        window.draw(text);
        text.setCharacterSize(20);
    }
}

void Game::drawVictoryInputScreen() {
    sf::RectangleShape overlay(sf::Vector2f(800, 500));
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    window.draw(overlay);

    text.setCharacterSize(28);
    text.setFillColor(sf::Color::Yellow);
    text.setString("VICTORY!");
    sf::FloatRect victoryBounds = text.getLocalBounds();
    text.setPosition(400 - victoryBounds.width / 2, 120);
    window.draw(text);

    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setString("Your time: " + std::to_string((int)gameTimer) + " seconds");
    sf::FloatRect timeBounds = text.getLocalBounds();
    text.setPosition(400 - timeBounds.width / 2, 170);
    window.draw(text);

    text.setString("Enter your name:");
    sf::FloatRect nameBounds = text.getLocalBounds();
    text.setPosition(400 - nameBounds.width / 2, 260);
    window.draw(text);

    sf::RectangleShape inputBox(sf::Vector2f(250, 35));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(275, 290);
    window.draw(inputBox);

    text.setFillColor(sf::Color::Black);
    text.setString(nameInput + "_");
    text.setPosition(280, 295);
    window.draw(text);

    text.setFillColor(sf::Color::White);
    text.setString("Press ENTER to save your score");
    sf::FloatRect enterBounds = text.getLocalBounds();
    text.setPosition(400 - enterBounds.width / 2, 350);
    window.draw(text);
}

void Game::handleVictoryInput(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 13) {
            if (nameInput.getSize() == 0) {
                nameInput = "Player";
            }
            playerName = nameInput;
            leaderboard.addRecord(playerName, gameTimer);
            victoryScreen = false;
            showLeaderboard = true;
        }
        else if (event.text.unicode == 8 && nameInput.getSize() > 0) {
            nameInput.erase(nameInput.getSize() - 1);
        }
        else if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
            if (nameInput.getSize() < 15) {
                nameInput += static_cast<char>(event.text.unicode);
            }
        }
    }
}

void Game::drawStartScreen() {
    sf::RectangleShape overlay(sf::Vector2f(800, 500));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Yellow);
    text.setString("MOUSE ON THE TRAIN");
    sf::FloatRect titleBounds = text.getLocalBounds();
    text.setPosition(400 - titleBounds.width / 2, 150);
    window.draw(text);

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition(300, 280);
    window.draw(startButton);

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setString("PRESS SPACE");
    sf::FloatRect buttonTextBounds = text.getLocalBounds();
    text.setPosition(400 - buttonTextBounds.width / 2, 293);
    window.draw(text);

    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setString("Use A/D to move, SPACE to jump, Q to attack, R to heal");
    sf::FloatRect controlsBounds = text.getLocalBounds();
    text.setPosition(400 - controlsBounds.width / 2, 400);
    window.draw(text);
}

void Game::run() {
    sf::Clock clock;
    sf::Clock gameClock;
    gameClock.restart();

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.033f) deltaTime = 0.033f;

        if (notificationTimer > 0.0f) {
            notificationTimer -= deltaTime;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                windowSize.x = event.size.width;
                windowSize.y = event.size.height;

                float windowAspect = (float)windowSize.x / windowSize.y;
                float gameAspect = 800.0f / 500.0f;

                sf::View newView;
                if (windowAspect > gameAspect) {
                    float width = 800 * (windowAspect / gameAspect);
                    newView.setSize(width, 500);
                    newView.setCenter(400, 250);
                }
                else {
                    float height = 500 * (gameAspect / windowAspect);
                    newView.setSize(800, height);
                    newView.setCenter(400, 250);
                }
                window.setView(newView);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                if (event.key.code == sf::Keyboard::F11) {
                    static bool isFullscreen = false;
                    isFullscreen = !isFullscreen;

                    if (isFullscreen) {
                        window.create(sf::VideoMode::getDesktopMode(), "Mouse on the Train", sf::Style::Fullscreen);
                    }
                    else {
                        window.create(sf::VideoMode(1920, 1080), "Mouse on the Train", sf::Style::Default);
                    }

                    window.setView(gameView);
                }

                if (showStartScreen && event.key.code == sf::Keyboard::Space) {
                    showStartScreen = false;
                    gameClock.restart();
                    continue;
                }
            }

            if (victoryScreen) {
                handleVictoryInput(event);
            }
        }

        if (timerRunning && !gameOver && !victory && !showStartScreen) {
            gameTimer = gameClock.getElapsedTime().asSeconds();
        }

        if (!gameOver && !victory && !victoryScreen && !showLeaderboard && !showStartScreen) {
            handleInput(deltaTime);
            player.update(deltaTime);
            updateMonsters(deltaTime);
            updateAttackCooldown(deltaTime);
            updateMarmaladeCooldown(deltaTime);
        }

        window.clear(sf::Color(50, 50, 50));

        if (!showStartScreen) {
            window.draw(backgroundSprite);
            drawGround();
            drawDoor();

            if (currentLocation == 2) monster1.draw(window);
            if (currentLocation == 3) monster2.draw(window);
            if (currentLocation == 4) monster3.draw(window);
            if (currentLocation == 5) monster4.draw(window);

            player.draw(window);
            drawUI();
        }
        else {
            drawStartScreen();
        }

        if (victoryScreen) {
            drawVictoryInputScreen();
        }

        if (showLeaderboard) {
            sf::RectangleShape overlay(sf::Vector2f(800, 500));
            overlay.setFillColor(sf::Color(0, 0, 0, 230));
            window.draw(overlay);
            leaderboard.draw(window, 250, 120);
            text.setCharacterSize(18);
            text.setFillColor(sf::Color::White);
            text.setString("Press ESC to exit");
            sf::FloatRect exitBounds = text.getLocalBounds();
            text.setPosition(400 - exitBounds.width / 2, 450);
            window.draw(text);
        }

        window.display();
    }
}