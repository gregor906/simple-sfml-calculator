#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable, public sf::Transformable {
public:
    Button(std::string text, sf::Font& font, unsigned int characterSize,
        sf::Vector2f position, sf::Vector2f size)
        : m_rect{ std::make_unique<sf::RectangleShape>(size) }, // Создаем прямоугольник кнопки
        m_text{ std::make_unique<sf::Text>(text, font, characterSize) } { // Создаем текст на кнопке

        m_rect->setPosition(position); // Задаем позицию кнопки
        m_rect->setFillColor(sf::Color(200, 200, 200)); // Серый фон
        m_rect->setOutlineColor(sf::Color::Black); // Черная обводка
        m_rect->setOutlineThickness(2); // Толщина обводки

        m_text->setPosition(position.x + 20, position.y + 20); // Текст с отступом внутри кнопки
        m_text->setFillColor(sf::Color::White); // Белый цвет текста
    }

    void pressEffect() { // Эффект нажатия — меняем цвет
        m_rect->setFillColor(sf::Color(150, 150, 150));
        m_rect->setOutlineColor(sf::Color(150, 150, 150));
    }

    void releaseEffect() { // Эффект отпускания — возвращаем исходный цвет
        m_rect->setFillColor(sf::Color(200, 200, 200));
        m_rect->setOutlineColor(sf::Color::Black);
    }

    std::string getText() const { // Получаем текст кнопки
        return m_text->getString();
    }

    sf::FloatRect getGlobalBounds() const { // Границы кнопки с учетом трансформаций
        return getTransform().transformRect(m_rect->getGlobalBounds());
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override { // Отрисовка кнопки
        states.transform *= getTransform();
        target.draw(*m_rect, states); // Рисуем прямоугольник
        target.draw(*m_text, states); // Рисуем текст
    }

    std::unique_ptr<sf::RectangleShape> m_rect; // Умный указатель на прямоугольник
    std::unique_ptr<sf::Text> m_text; // Умный указатель на текст
};