#include "Player.h"
#include <iostream>

Player::Player() : health(100), marmalade(3), velocityY(0),
isGrounded(false), canMove(true), useCooldown(0.0f),
isAttacking(false), attackAnimTimer(0.0f) {
    body.setSize(sf::Vector2f(50, 76));
    body.setFillColor(sf::Color::White);
    body.setPosition(100, 400 - body.getSize().y);
}

void Player::setTexture(const std::string& basePath) {
    if (!textureIdle.loadFromFile("mouse.png")) {
        std::cout << "Failed to load mouse.png" << std::endl;
    }
    if (!textureJump.loadFromFile("mouse_jump.png")) {
        std::cout << "Failed to load mouse_jump.png" << std::endl;
    }
    if (!textureAttack.loadFromFile("mouse_atack.png")) {
        std::cout << "Failed to load mouse_atack.png" << std::endl;
    }
    if (!textureStep.loadFromFile("mouse_step.png")) {
        std::cout << "Failed to load mouse_step.png" << std::endl;
    }

    body.setTexture(&textureIdle);
    body.setSize(sf::Vector2f(50, 76));
    body.setPosition(body.getPosition().x, 400 - body.getSize().y);
}

void Player::update(float deltaTime) {
    if (!canMove) return;

    if (useCooldown > 0.0f) {
        useCooldown -= deltaTime;
    }

    if (isAttacking) {
        attackAnimTimer -= deltaTime;
        if (attackAnimTimer <= 0) {
            isAttacking = false;
            body.setTexture(&textureIdle);
        }
    }

    velocityY += 980.0f * deltaTime;
    body.move(0, velocityY * deltaTime);

    if (body.getPosition().y + body.getSize().y >= 400) {
        body.setPosition(body.getPosition().x, 400 - body.getSize().y);
        velocityY = 0;
        isGrounded = true;
    }
    else {
        isGrounded = false;
    }

    if (body.getPosition().x < 0) {
        body.setPosition(0, body.getPosition().y);
    }
    if (body.getPosition().x > 730) {
        body.setPosition(730, body.getPosition().y);
    }

    if (!isAttacking) {
        if (!isGrounded) {
            body.setTexture(&textureJump);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            static float stepTimer = 0;
            stepTimer += deltaTime;
            if (stepTimer > 0.15f) {
                stepTimer = 0;
                body.setTexture((body.getTexture() == &textureStep) ? &textureIdle : &textureStep);
            }
        }
        else {
            body.setTexture(&textureIdle);
        }
    }
}

void Player::moveLeft(float deltaTime) {
    if (canMove) body.move(-250.0f * deltaTime, 0);
}

void Player::moveRight(float deltaTime) {
    if (canMove) body.move(250.0f * deltaTime, 0);
}

void Player::jump() {
    if (canMove && isGrounded) {
        velocityY = -400.0f;
        isGrounded = false;
    }
}

void Player::attack() {
    if (!canMove) return;
    isAttacking = true;
    attackAnimTimer = 0.3f;
    body.setTexture(&textureAttack);
}

void Player::useMarmalade() {
    if (useCooldown > 0.0f) {
        std::cout << "Marmalade on cooldown!" << std::endl;
        return;
    }

    if (marmalade > 0) {
        marmalade--;
        health.heal(10);
        useCooldown = 0.8f;
        std::cout << "Healed HP: " << health.getCurrent() << " | Marmalade left: " << marmalade << std::endl;
    }
    else {
        std::cout << "No marmalade!" << std::endl;
    }
}

void Player::takeDamage(int dmg) {
    health.takeDamage(dmg);
    std::cout << "Got damage " << dmg << "! HP: " << health.getCurrent() << std::endl;
}

void Player::addMarmalade() {
    if (marmalade < 3) {
        marmalade++;
        std::cout << "Marmalade found! Now: " << marmalade << std::endl;
    }
    else {
        std::cout << "Max marmalade (3)" << std::endl;
    }
}

void Player::resetPosition() {
    body.setPosition(100, 400 - body.getSize().y);
    velocityY = 0;
    isGrounded = true;
}

int Player::getHp() const {
    return health.getCurrent();
}

int Player::getMarmalade() const {
    return marmalade;
}

float Player::getX() const {
    return body.getPosition().x;
}

float Player::getY() const {
    return body.getPosition().y;
}

sf::FloatRect Player::getBounds() const {
    return body.getGlobalBounds();
}

bool Player::canPlayerMove() const {
    return canMove;
}

Health& Player::getHealth() {
    return health;
}

void Player::setX(float x) {
    body.setPosition(x, body.getPosition().y);
}

void Player::setCanMove(bool move) {
    canMove = move;
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(body);
}