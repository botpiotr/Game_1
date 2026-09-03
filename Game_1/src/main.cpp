#include "mainwindow.h"

#include <QApplication>

#include "settings/gamesettings.h"

#include "screens/screenmanager.h"
#include "screens/mainmenuscreen.h"
#include "screens/optionsscreen.h"
#include "screens/creditsscreen.h"
#include "screens/gameboardscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.resize(800, 600);
    window.setWindowTitle("Game_1");

    // Applique immédiatement le mode plein écran/fenêtré chargé depuis
    // settings.json, et reste synchronisé si l'utilisateur le change
    // plus tard depuis l'écran Options.
    GameSettings &settings = GameSettings::instance();
    if (settings.fullscreen()) {
        window.showFullScreen();
    }
    QObject::connect(&settings, &GameSettings::fullscreenChanged, &window, [&window](bool fullscreen) {
        if (fullscreen) {
            window.showFullScreen();
        } else {
            window.showNormal();
        }
    });

    auto *screenManager = new ScreenManager(&window);
    screenManager->registerScreen(ScreenId::MainMenu, new MainMenuScreen());
    screenManager->registerScreen(ScreenId::Options, new OptionsScreen());
    screenManager->registerScreen(ScreenId::Credits, new CreditsScreen());
    screenManager->registerScreen(ScreenId::GameBoard, new GameBoardScreen());

    window.setCentralWidget(screenManager);
    screenManager->showScreen(ScreenId::MainMenu);

    window.show();

    return a.exec();
}
