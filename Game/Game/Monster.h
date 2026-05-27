#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Health.h"

class Monster {
private:
    sf::RectangleShape body;
    sf::Texture textureIdle;
    sf::Texture textureStep;
    sf::Texture textureAttack;
    Health health;
    int damage;
    bool alive;
    std::string name;
    float speed;
    float attackTimer;
    float animTimer;
    bool isAttacking;
    float attackAnimTimer;

public:
    Monster(const std::string& n, int hp, int dmg, const std::string& texturePath, float spd = 90.0f);

    void update(float deltaTime, float playerX);
    bool isCloseTo(float playerX, float playerY) const;
    bool readyToAttack();

    void takeDamage(int dmg);
    void reset();
    int getHp() const;
    int getMaxHp() const;
    int getDamage() const;
    bool isAlive() const;
    std::string getName() const;
    sf::FloatRect getBounds() const;
    float getX() const;
    float getY() const;
    void draw(sf::RenderWindow& window) const;
};
