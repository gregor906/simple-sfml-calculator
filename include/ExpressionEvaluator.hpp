#pragma once
#include <string>
#include <string_view>
#include <stdexcept>
#include <charconv>

class ExpressionEvaluator {
public:
    static double evaluate(std::string_view expression) { // Вычисляем выражение
        size_t pos = 0;
        return parseExpression(expression, pos);
    }

private:
    static double parseExpression(std::string_view expr, size_t& pos) { // Разбираем выражение (+, -)
        double result = parseTerm(expr, pos);
        while (pos < expr.length()) {
            char op = expr[pos];
            if (op != '+' && op != '-') break;
            pos++;
            double term = parseTerm(expr, pos);
            result = (op == '+') ? result + term : result - term;
        }
        return result;
    }

    static double parseTerm(std::string_view expr, size_t& pos) { // Разбираем члены (*, /)
        double result = parseFactor(expr, pos);
        while (pos < expr.length()) {
            char op = expr[pos];
            if (op != '*' && op != '/') break;
            pos++;
            double factor = parseFactor(expr, pos);
            if (op == '*') result *= factor;
            else if (factor == 0) throw std::invalid_argument("Деление на ноль!");
            else result /= factor;
        }
        return result;
    }

    static double parseFactor(std::string_view expr, size_t& pos) { // Разбираем множители (числа, скобки)
        skipWhitespace(expr, pos);
        if (pos >= expr.length()) throw std::invalid_argument("Некорректное выражение");

        if (expr[pos] == '(') { // Обработка скобок
            pos++;
            double result = parseExpression(expr, pos);
            skipWhitespace(expr, pos);
            if (pos >= expr.length() || expr[pos] != ')')
                throw std::invalid_argument("Нет закрывающей скобки");
            pos++;
            return result;
        }

        double result{};
        auto [ptr, ec] = std::from_chars(expr.data() + pos, // Парсим число
            expr.data() + expr.length(),
            result);
        if (ec != std::errc()) throw std::invalid_argument("Некорректное число");
        pos = ptr - expr.data();
        return result;
    }

    static void skipWhitespace(std::string_view expr, size_t& pos) { // Пропускаем пробелы
        while (pos < expr.length() && std::isspace(expr[pos])) pos++;
    }
};