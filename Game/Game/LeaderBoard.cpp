#include "Leaderboard.h"
#include <iostream>

Leaderboard::Leaderboard() {
    load();
}

void Leaderboard::load() {
    records.clear();
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string name;
        float time;
        while (file >> name >> time) {
            records.push_back(Record(name, time));
        }
        file.close();
    }
}

void Leaderboard::save() {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& r : records) {
            file << r.name << " " << r.time << std::endl;
        }
        file.close();
    }
}

void Leaderboard::addRecord(const std::string& name, float time) {
    records.push_back(Record(name, time));

    std::sort(records.begin(), records.end(),
        [](const Record& a, const Record& b) { return a.time < b.time; });

    if (records.size() > 4) {
        records.resize(4);
    }

    save();
}

bool Leaderboard::isHighScore(float time) {
    if (records.size() < 4) return true;
    return time < records.back().time;
}

void Leaderboard::draw(sf::RenderWindow& window, float x, float y) {
    if (!font.loadFromFile("font.ttf")) return;

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Yellow);

    text.setString("=== LEADERBOARD ===");
    sf::FloatRect titleBounds = text.getLocalBounds();
    text.setPosition(400 - titleBounds.width / 2, y);
    window.draw(text);

    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    if (records.empty()) {
        text.setString("No records");
        sf::FloatRect emptyBounds = text.getLocalBounds();
        text.setPosition(400 - emptyBounds.width / 2, y + 50);
        window.draw(text);
        return;
    }

    for (int i = 0; i < (int)records.size(); i++) {
        std::string entry = std::to_string(i + 1) + ". " + records[i].name +
            " - " + std::to_string((int)records[i].time) + " sec";

        text.setString(entry);
        sf::FloatRect entryBounds = text.getLocalBounds();
        text.setPosition(400 - entryBounds.width / 2, y + 50 + i * 35);
        window.draw(text);
    }
}