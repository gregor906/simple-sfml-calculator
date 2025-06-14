#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable, public sf::Transformable {
public:
    Button(std::string text, sf::Font& font, unsigned int characterSize,
        sf::Vector2f position, sf::Vector2f size)
        : m_rect{ std::make_unique<sf::RectangleShape>(size) },
        m_text{ std::make_unique<sf::Text>(text, font, characterSize) } { // Создаем текст на кнопке

        m_rect->setPosition(position); // Позиция
        m_rect->setFillColor(sf::Color(200, 200, 200));
        m_rect->setOutlineColor(sf::Color::Black);
        m_rect->setOutlineThickness(2);

        m_text->setPosition(position.x + 20, position.y + 20);
        m_text->setFillColor(sf::Color::White);
    }

    void pressEffect() {
        m_rect->setFillColor(sf::Color(150, 150, 150));
        m_rect->setOutlineColor(sf::Color(150, 150, 150));
    }

    void releaseEffect() {
        m_rect->setFillColor(sf::Color(200, 200, 200));
        m_rect->setOutlineColor(sf::Color::Black);
    }

    std::string getText() const {
        return m_text->getString();
    }

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(m_rect->getGlobalBounds());
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(*m_rect, states);
        target.draw(*m_text, states);
    }

    std::unique_ptr<sf::RectangleShape> m_rect;
    std::unique_ptr<sf::Text> m_text;
};