#include "Monster.h"
#include <iostream>
#include <cmath>

Monster::Monster(const std::string& n, int hp, int dmg, const std::string& texturePath, float spd)
    : health(hp), damage(dmg), alive(true), name(n), speed(spd),
    attackTimer(0.0f), animTimer(0.0f), isAttacking(false), attackAnimTimer(0.0f) {

    std::string baseName = texturePath.substr(0, texturePath.find_last_of('.'));
    std::string stepPath = baseName + "_step.png";
    std::string attackPath = baseName + "_atack.png";

    if (!textureIdle.loadFromFile(texturePath)) {
        textureIdle.create(40, 40);
    }

    textureStep.loadFromFile(stepPath);
    textureAttack.loadFromFile(attackPath);

    sf::Vector2u texSize = textureIdle.getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        body.setSize(sf::Vector2f(texSize.x * 2.0f, texSize.y * 2.0f));
    }
    else {
        body.setSize(sf::Vector2f(36, 76));
    }

    body.setTexture(&textureIdle);
    body.setPosition(500, 400 - body.getSize().y);
}

void Monster::update(float deltaTime, float playerX) {
    if (!alive) return;
    if (attackTimer > 0.0f) attackTimer -= deltaTime;
    if (animTimer > 0.0f) animTimer -= deltaTime;

    if (isAttacking) {
        attackAnimTimer -= deltaTime;
        if (attackAnimTimer <= 0) {
            isAttacking = false;
            body.setTexture(&textureIdle);
        }
    }

    float dx = playerX - body.getPosition().x;
    if (std::abs(dx) > 50.0f && !isAttacking) {
        float dir = (dx > 0) ? 1.0f : -1.0f;
        body.move(dir * speed * deltaTime, 0);

        if (animTimer <= 0 && textureStep.getSize().x > 0) {
            animTimer = 0.2f;
            body.setTexture((body.getTexture() == &textureStep) ? &textureIdle : &textureStep);
        }
    }
    else if (!isAttacking) {
        body.setTexture(&textureIdle);
    }

    if (body.getPosition().x < 0) body.setPosition(0, body.getPosition().y);
    if (body.getPosition().x > 730 - body.getSize().x) {
        body.setPosition(730 - body.getSize().x, body.getPosition().y);
    }
}

bool Monster::isCloseTo(float playerX, float playerY) const {
    return std::abs(body.getPosition().x - playerX) < 55.0f &&
        std::abs(body.getPosition().y - playerY) < 60.0f;
}

bool Monster::readyToAttack() {
    if (!alive || attackTimer > 0.0f) return false;
    attackTimer = 1.4f;

    if (textureAttack.getSize().x > 0) {
        isAttacking = true;
        attackAnimTimer = 0.3f;
        body.setTexture(&textureAttack);
    }

    return true;
}

void Monster::takeDamage(int dmg) {
    if (!alive) return;
    health.takeDamage(dmg);
    std::cout << name << " Got damage " << dmg << "! Current HP: " << health.getCurrent() << std::endl;

    if (!health.isAlive()) {
        alive = false;
        std::cout << name << " defeated!\n";
    }
}

void Monster::reset() {
    health = Health(health.getMax());
    alive = true;
    attackTimer = 0.0f;
    body.setPosition(500, 400);
}

int Monster::getHp() const { return health.getCurrent(); }
int Monster::getMaxHp() const { return health.getMax(); }
int Monster::getDamage() const { return damage; }
bool Monster::isAlive() const { return alive; }
std::string Monster::getName() const { return name; }
sf::FloatRect Monster::getBounds() const { return body.getGlobalBounds(); }
float Monster::getX() const { return body.getPosition().x; }
float Monster::getY() const { return body.getPosition().y; }
void Monster::draw(sf::RenderWindow& window) const { if (alive) window.draw(body); }