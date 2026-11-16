cmake_minimum_required(VERSION 3.28)

project(Tekku LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML)

add_executable(tekku
    main.cpp
    pet.cpp
    feed.cpp
    play.cpp
    sleep.cpp
    shower.cpp
)

target_compile_features(tekku PRIVATE cxx_std_17)

target_link_libraries(tekku PRIVATE
    SFML::Graphics
    SFML::Audio
)