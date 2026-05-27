#pragma once

class Health {
private:
    int current;
    int max;

public:
    Health(int maxHp);

    void takeDamage(int dmg);
    void heal(int amount);
    bool isAlive() const;
    int getCurrent() const;
    int getMax() const;
    float getPercent() const;
};