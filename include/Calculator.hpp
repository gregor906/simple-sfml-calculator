#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "ExpressionEvaluator.hpp"
#include "History.hpp"

class Calculator : public sf::Drawable, public sf::Transformable {
public:
    explicit Calculator(sf::Font& font) : history(font, 18) {
        display = std::make_unique<sf::RectangleShape>(sf::Vector2f(360, 50));
        display->setPosition(20, 150);
        display->setFillColor(sf::Color(173, 216, 230));
        display->setOutlineColor(sf::Color::Black);
        display->setOutlineThickness(2);

        displayText = std::make_unique<sf::Text>("", font, 30);
        displayText->setPosition(30, 150);
        displayText->setFillColor(sf::Color::Black);

        history.setPosition(sf::Vector2f(20, 10));
        history.setSize(360, 120);

        windowBackground = std::make_unique<sf::RectangleShape>(sf::Vector2f(400, 800));
        windowBackground->setFillColor(sf::Color::White);

        const std::vector<std::string> labels = { // Список меток для кнопок
            "7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-",
            "0", "C", "=", "+", "(", ")", "<<<"
        };

        buttons.reserve(labels.size());
        for (size_t i = 0; i < labels.size(); ++i) { // Создаем кнопки в цикле
            buttons.emplace_back(std::make_unique<Button>(
                labels[i], font, 24,
                sf::Vector2f(20 + (i % 4) * 90, 300 + (i / 4) * 90),
                sf::Vector2f(80, 80)
            ));
        }
    }

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) { // Обработка событий
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            for (const auto& button : buttons) {
                if (button->getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                    button->pressEffect();
                    processInput(button->getText());
                    displayText->setString(input);
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
            for (const auto& button : buttons) {
                button->releaseEffect();
            }
        }
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override { // Отрисовка калькулятора
        states.transform *= getTransform();
        target.draw(*windowBackground, states);
        target.draw(history, states);
        target.draw(*display, states);
        target.draw(*displayText, states);
        for (const auto& button : buttons) {
            target.draw(*button, states);
        }
    }

    History history;

    void processInput(const std::string& text) {
        using namespace std::string_literals;
        if (text == "C"s) {
            input.clear();
        }
        else if (text == "="s) {
            try {
                std::string originalInput = input;
                double result = ExpressionEvaluator::evaluate(input);
                input = std::to_string(result);
                if (input.ends_with(".000000")) {
                    input = input.substr(0, input.find('.'));
                }
                history.addEntry(originalInput, input);
            }
            catch (const std::exception&) {
                input = "Error"s;
            }
        }
        else if (text == "<<<"s) { // Удаление последнего символа
            if (!input.empty()) {
                input.pop_back();
            }
        }
        else if (text == "+"s || text == "-"s || text == "*"s || text == "/"s) {
            std::string_view ops = "+-*/";
            if (!input.empty() && ops.find(input.back()) == std::string_view::npos && input.length() < 19) {
                input += text;
            }
        }
        else if (input.length() < 19) {
            input += text;
        }
        displayText->setString(input);
    }

    std::unique_ptr<sf::RectangleShape> windowBackground;
    std::unique_ptr<sf::RectangleShape> display;
    std::unique_ptr<sf::Text> displayText;
    std::vector<std::unique_ptr<Button>> buttons;
    std::string input;
};