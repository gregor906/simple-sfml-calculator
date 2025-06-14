#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "ExpressionEvaluator.hpp"

class Calculator : public sf::Drawable, public sf::Transformable {
public:
    explicit Calculator(sf::Font& font) { // Конструктор принимает шрифт
        display = std::make_unique<sf::RectangleShape>(sf::Vector2f(360, 50)); // Создаем дисплей
        display->setPosition(20, 20); // Позиция дисплея
        display->setFillColor(sf::Color(173, 216, 230)); // Голубой фон
        display->setOutlineColor(sf::Color::Black); // Черная обводка
        display->setOutlineThickness(2); // Толщина обводки

        displayText = std::make_unique<sf::Text>("", font, 30); // Текст на дисплее
        displayText->setPosition(30, 30); // Позиция текста
        displayText->setFillColor(sf::Color::Black); // Черный цвет текста

        windowBackground = std::make_unique<sf::RectangleShape>(sf::Vector2f(400, 600)); // Фон окна
        windowBackground->setFillColor(sf::Color::White); // Белый цвет фона

        const std::vector<std::string> labels = { // Список меток для кнопок
            "7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-",
            "0", "C", "=", "+", "(", ")", "<<<"
        };

        buttons.reserve(labels.size()); // Резервируем место под кнопки
        for (size_t i = 0; i < labels.size(); ++i) { // Создаем кнопки в цикле
            buttons.emplace_back(std::make_unique<Button>(
                labels[i], font, 24,
                sf::Vector2f(20 + (i % 4) * 90, 100 + (i / 4) * 90),
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
                    button->pressEffect(); // Анимация нажатия
                    processInput(button->getText()); // Обрабатываем ввод
                    displayText->setString(input); // Обновляем дисплей
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
            for (const auto& button : buttons) {
                button->releaseEffect(); // Возвращаем цвет
            }
        }
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override { // Отрисовка калькулятора
        states.transform *= getTransform();
        target.draw(*windowBackground, states); // Рисуем фон
        target.draw(*display, states); // Рисуем дисплей
        target.draw(*displayText, states); // Рисуем текст
        for (const auto& button : buttons) {
            target.draw(*button, states); // Рисуем все кнопки
        }
    }

    void processInput(const std::string& text) { // Логика обработки ввода
        using namespace std::string_literals;
        if (text == "C"s) { // Очистка
            input.clear();
        }
        else if (text == "="s) { // Вычисление результата
            try {
                double result = ExpressionEvaluator::evaluate(input);
                input = std::to_string(result);
                if (input.ends_with(".000000")) { // Убираем лишние нули
                    input = input.substr(0, input.find('.'));
                }
            }
            catch (const std::exception&) {
                input = "Error"s; // Ошибка при вычислении
            }
        }
        else if (text == "<<<"s) { // Удаление последнего символа
            if (!input.empty()) {
                input.pop_back();
            }
        }
        else if (text == "+"s || text == "-"s || text == "*"s || text == "/"s) { // Операторы
            std::string_view ops = "+-*/";
            if (!input.empty() && ops.find(input.back()) == std::string_view::npos && input.length() < 19) {
                input += text;
            }
        }
        else if (input.length() < 19) { // Добавляем символ, если не превышен лимит
            input += text;
        }
        displayText->setString(input); // Обновляем текст на дисплее
    }

    std::unique_ptr<sf::RectangleShape> windowBackground; // Фон окна
    std::unique_ptr<sf::RectangleShape> display; // Дисплей
    std::unique_ptr<sf::Text> displayText; // Текст дисплея
    std::vector<std::unique_ptr<Button>> buttons; // Вектор кнопок
    std::string input; // Текущий ввод
};