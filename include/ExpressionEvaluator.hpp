#pragma once
#include <string>
#include <string_view>
#include <stdexcept>
#include <charconv>


     /**
     * @class ExpressionEvaluator
     * @brief Класс для вычисления математических выражений, заданных строкой.
     *
     * Поддерживает базовые операции: +, -, *, /, скобки () и пробелы между символами.
     * Генерирует исключения при синтаксических ошибках или делении на ноль.
     */
class ExpressionEvaluator {
public:
     /**
     * @brief Вычисляет значение математического выражения.
     * @param expression Строка с выражением (например, "2*(3+4)").
     * @return Результат вычисления типа double.
     * @throw std::invalid_argument При ошибках парсинга или делении на ноль.
     *
     * Пример использования:
     * @code
     * double result = ExpressionEvaluator::evaluate("(2+3)*4");
     * @endcode
     */
    static double evaluate(std::string_view expression) { // Вычисляем выражение
        size_t pos = 0;
        return parseExpression(expression, pos);
    }

private:
     /**
     * @brief Парсит выражение, обрабатывая операции + и -.
     * @param expr Строка выражения.
     * @param pos Текущая позиция парсинга (используется рекурсивно).
     * @return Результат подвыражения.
     * @throw std::invalid_argument При ошибках формата.
     */
    static double parseExpression(std::string_view expr, size_t& pos) {
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

     /**
     * @brief Парсит термы, обрабатывая операции * и /.
     * @param expr Строка выражения.
     * @param pos Текущая позиция парсинга.
     * @throw std::invalid_argument При делении на ноль.
     */
    static double parseTerm(std::string_view expr, size_t& pos) {
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

     /**
     * @brief Парсит множители (числа или подвыражения в скобках).
     * @param expr Строка выражения.
     * @param pos Текущая позиция парсинга.
     * @throw std::invalid_argument При некорректных числах или незакрытых скобках.
     */
    static double parseFactor(std::string_view expr, size_t& pos) {
        skipWhitespace(expr, pos);
        if (pos >= expr.length()) throw std::invalid_argument("Некорректное выражение");

        if (expr[pos] == '(') {
            pos++;
            double result = parseExpression(expr, pos);
            skipWhitespace(expr, pos);
            if (pos >= expr.length() || expr[pos] != ')')
                throw std::invalid_argument("Нет закрывающей скобки");
            pos++;
            return result;
        }

        double result{};
        auto [ptr, ec] = std::from_chars(expr.data() + pos,
            expr.data() + expr.length(),
            result);
        if (ec != std::errc()) throw std::invalid_argument("Некорректное число");
        pos = ptr - expr.data();
        return result;
    }

     /**
     * @brief Пропускает пробельные символы.
     * @param expr Строка выражения.
     * @param pos Текущая позиция (изменяется в процессе).
     */
    static void skipWhitespace(std::string_view expr, size_t& pos) {
        while (pos < expr.length() && std::isspace(expr[pos])) pos++;
    }
};