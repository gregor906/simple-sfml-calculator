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

# Заголовочные файлы
set(HEADERS
    include/Calculator.hpp
    include/Button.hpp
    include/ExpressionEvaluator.hpp
)

# Создание исполняемого файла
add_executable(calculator ${SOURCES} ${HEADERS})

target_include_directories(calculator PRIVATE include)

# Подключение SFML
target_link_libraries(calculator sfml-graphics sfml-window sfml-system)