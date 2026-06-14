cmake_minimum_required(VERSION 3.16)

project(counter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) # автоматический запуск moc для классов с Q_OBJECT

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(counter
    main.cpp
    win.cpp
    win.h)

target_link_libraries(counter PRIVATE Qt5::Widgets)

# Исходники хранятся в кодировке Windows-1251 (как в методичке, пример 2.8),
# поэтому указываем компилятору входную/исполняемую кодировку, чтобы
# QTextCodec::codecForName("Windows-1251")->toUnicode(...) декодировал строки верно.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(counter PRIVATE
        -finput-charset=windows-1251
        -fexec-charset=windows-1251)
endif()
