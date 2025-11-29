cmake_minimum_required(VERSION 3.15)
project(ECHOES VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler-spezifische Flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies (SDL2, SDL2_image, SDL2_mixer, SDL2_ttf)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Optional: OpenCV für Kamera (später)
find_package(OpenCV QUIET)
if(OpenCV_FOUND)
    message(STATUS "OpenCV gefunden - Kamera-Support aktiviert")
    add_compile_definitions(HAS_CAMERA_SUPPORT)
endif()

# Source Files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/MetaHorror.cpp
    src/PermissionManager.cpp
    src/CorruptionSystem.cpp
    src/Player.cpp
    src/Level.cpp
    src/AudioManager.cpp
    src/UIHelper.cpp
    src/MainMenu.cpp
    src/PauseMenu.cpp
    src/DialogSystem.cpp
    src/FakeBlueScreen.cpp
    src/JumpscareSystem.cpp
    src/Enemy.cpp
    src/NPC.cpp
    src/Boss.cpp
)

set(HEADERS
    src/Game.h
    src/GameState.h
    src/MetaHorror.h
    src/PermissionManager.h
    src/CorruptionSystem.h
    src/Player.h
    src/Level.h
    src/AudioManager.h
    src/UIHelper.h
    src/MainMenu.h
    src/PauseMenu.h
    src/DialogSystem.h
    src/FakeBlueScreen.h
    src/JumpscareSystem.h
    src/Enemy.h
    src/NPC.h
    src/Boss.h
)

# Executable
add_executable(ECHOES ${SOURCES} ${HEADERS})

# Include Directories
target_include_directories(ECHOES PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Libraries
target_link_libraries(ECHOES PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

# OpenCV optional
if(OpenCV_FOUND)
    target_link_libraries(ECHOES PRIVATE ${OpenCV_LIBS})
endif()

# Copy assets to build directory
add_custom_command(TARGET ECHOES POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ECHOES>/assets
)

# Install rules
install(TARGETS ECHOES DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)
