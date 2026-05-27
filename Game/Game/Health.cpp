#include "Health.h"

Health::Health(int maxHp) : max(maxHp), current(maxHp) {}

void Health::takeDamage(int dmg) {
    current -= dmg;
    if (current < 0) current = 0;
}

void Health::heal(int amount) {
    current += amount;
    if (current > max) current = max;
}

bool Health::isAlive() const {
    return current > 0;
}

int Health::getCurrent() const {
    return current;
}

int Health::getMax() const {
    return max;
}

float Health::getPercent() const {
    return (float)current / max;
}