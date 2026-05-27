#pragma once
#include <SFML/Graphics.hpp>
#include "Health.h"

class Player {
private:
    sf::RectangleShape body;
    sf::Texture texture;
    sf::Texture textureIdle;
    sf::Texture textureJump;
    sf::Texture textureAttack;
    sf::Texture textureStep;
    Health health;
    int marmalade;
    float velocityY;
    bool isGrounded;
    bool canMove;
    float useCooldown;

    bool isAttacking;
    float attackAnimTimer;

public:
    Player();

    void setTexture(const std::string& texturePath);
    void update(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void jump();
    void useMarmalade();
    void takeDamage(int dmg);
    void addMarmalade();
    void resetPosition();
    void attack();

    int getHp() const;
    int getMarmalade() const;
    float getX() const;
    float getY() const;
    sf::FloatRect getBounds() const;
    bool canPlayerMove() const;
    Health& getHealth();

    void setX(float x);
    void setCanMove(bool move);

    void draw(sf::RenderWindow& window) const;
};