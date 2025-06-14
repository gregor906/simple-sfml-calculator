#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class History : public sf::Drawable, public sf::Transformable {
public:
    History(sf::Font& font, unsigned int characterSize = 18)
        : m_font(font), m_characterSize(characterSize) {

        m_background.setSize(sf::Vector2f(360,150));
        m_background.setFillColor(sf::Color(240, 240, 240));
        m_background.setOutlineColor(sf::Color::Black);
        m_background.setOutlineThickness(1);

        m_title.setFont(m_font);
        m_title.setString("History:");
        m_title.setCharacterSize(m_characterSize);
        m_title.setFillColor(sf::Color::Black);
    }

    void addEntry(const std::string& expression, const std::string& result) {
        m_entries.emplace_back(expression + " = " + result);
        if (m_entries.size() > 3) {
            m_entries.erase(m_entries.begin());
        }
        updateDisplay();
    }

    void clear() {
        m_entries.clear();
        updateDisplay();
    }

    void setPosition(const sf::Vector2f& position) {
        m_background.setPosition(position);
        m_title.setPosition(position.x + 10, position.y + 10);
        updateDisplay();
    }

    void setSize(float width, float height) {
        m_background.setSize(sf::Vector2f(width, height));
        updateDisplay();
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_background, states);
        target.draw(m_title, states);
        for (const auto& text : m_displayTexts) {
            target.draw(text, states);
        }
    }

    void updateDisplay() {
        m_displayTexts.clear();
        for (size_t i = 0; i < m_entries.size(); ++i) {
            sf::Text text;
            text.setFont(m_font);
            text.setString(m_entries[i]);
            text.setCharacterSize(m_characterSize);
            text.setFillColor(sf::Color::Black);
            text.setPosition(m_background.getPosition().x + 10,
                m_background.getPosition().y + 40 + i * 30);
            m_displayTexts.push_back(text);
        }
    }

    sf::Font& m_font;
    unsigned int m_characterSize;
    sf::RectangleShape m_background;
    sf::Text m_title;
    std::vector<std::string> m_entries;
    std::vector<sf::Text> m_displayTexts;
};