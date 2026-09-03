cmake_minimum_required(VERSION 3.16)

project(Game_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

# find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)
# --- Bibliothèque principale (logique + écrans, réutilisée par l'exécutable et les tests) ---
add_library(game1_core
    # src/hextileitem.cpp
    # src/hextileitem.h

    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui

    src/settings/gamesettings.h
    src/settings/gamesettings.cpp

    src/screens/abstractscreen.h
    src/screens/screenmanager.cpp
    src/screens/screenmanager.h
    src/screens/mainmenuscreen.cpp
    src/screens/mainmenuscreen.h
    src/screens/optionsscreen.cpp
    src/screens/optionsscreen.h
    src/screens/creditsscreen.cpp
    src/screens/creditsscreen.h
    src/screens/gameboardscreen.cpp
    src/screens/gameboardscreen.h
)
target_include_directories(game1_core PUBLIC src)
target_link_libraries(game1_core PUBLIC Qt6::Widgets)

# --- Exécutable principal ---
add_executable(Game_1
    src/main.cpp
)
target_link_libraries(Game_1 PRIVATE game1_core)

# --- Installation (utilisée pour le packaging en CI) ---
install(TARGETS Game_1
    RUNTIME DESTINATION bin
)

# --- Tests unitaires ---
# Le sous-dossier gère sa propre configuration (cible, find_package Test, etc.)
option(BUILD_TESTING "Construire les tests unitaires" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
