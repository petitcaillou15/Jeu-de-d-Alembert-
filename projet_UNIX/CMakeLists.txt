cmake_minimum_required(VERSION 3.28)

project(Projet_UNIX LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(main
    main.cpp
    game.cpp
    joueur.cpp
)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system)