cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 20)

# Настройка SFML

set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6.1 REQUIRED COMPONENTS graphics window system)

# Исходные файлы
set(SOURCES
    src/main.cpp
)

set(TEST_SOURCES
    src/tests.cpp
)

# Заголовочные файлы
set(HEADERS
    include/Calculator.hpp
    include/Button.hpp
    include/ExpressionEvaluator.hpp
    include/History.hpp
    include/doctest.h
)

# Создание исполняемого файла
add_executable(calculator ${SOURCES} ${HEADERS})
target_include_directories(calculator PRIVATE include)
target_link_libraries(calculator PRIVATE sfml-graphics sfml-window sfml-system)

#Тесты
add_executable(Tests ${TEST_SOURCES} ${HEADERS})
target_include_directories(Tests PRIVATE include)
target_link_libraries(Tests PRIVATE sfml-system)