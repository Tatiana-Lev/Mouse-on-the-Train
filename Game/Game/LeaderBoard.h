#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct Record {
    std::string name;
    float time;

    Record() : name(""), time(0.0f) {}
    Record(const std::string& n, float t) : name(n), time(t) {}
};

class Leaderboard {
private:
    std::vector<Record> records;
    std::string filename = "leaderboard.txt";
    sf::Font font;
    sf::Text text;

public:
    Leaderboard();
    void load();
    void save();
    void addRecord(const std::string& name, float time);
    void draw(sf::RenderWindow& window, float x, float y);
    bool isHighScore(float time);
    int getRecordCount() const { return records.size(); }
};