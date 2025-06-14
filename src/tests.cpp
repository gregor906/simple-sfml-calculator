#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ExpressionEvaluator.hpp"

TEST_CASE("Testing ExpressionEvaluator") {
    SUBCASE("Basic arithmetic") {
        CHECK(ExpressionEvaluator::evaluate("2+2") == 4);
        CHECK(ExpressionEvaluator::evaluate("5*3") == 15);
        CHECK(ExpressionEvaluator::evaluate("10/2") == 5);
    }

    SUBCASE("Division by zero") {
        CHECK_THROWS_AS(ExpressionEvaluator::evaluate("5/0"), std::invalid_argument);
    }

    SUBCASE("Parentheses") {
        CHECK(ExpressionEvaluator::evaluate("(2+3)*4") == 20);
    }

    SUBCASE("Invalid input") {
        CHECK_THROWS_AS(ExpressionEvaluator::evaluate("2+"), std::invalid_argument);
    }
}